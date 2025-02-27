#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char *program_name)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  Interactive mode: %s\n", program_name);
	fprintf(stderr, "  Command line mode: %s <filename> [libraries...]\n", program_name);
	fprintf(stderr, "Example:\n");
	fprintf(stderr, "  %s myprogram readline ncurses\n", program_name);
}

int main(int argc, char **argv)
{
	// Build itself
	NOB_GO_REBUILD_URSELF(argc, argv);

	char file[256] = {0};
	char libraries[256] = {0};

	// Handle command line arguments if provided
	if (argc > 1)
	{
		// Get filename from first argument
		if (strlen(argv[1]) >= sizeof(file))
		{
			fprintf(stderr, "ERROR: Filename too long\n");
			return 1;
		}
		strcpy(file, argv[1]);

		// Collect libraries if provided
		if (argc > 2)
		{
			size_t total_len = 0;
			for (int i = 2; i < argc; i++)
			{
				total_len += strlen(argv[i]) + 1; // +1 for space
				if (total_len >= sizeof(libraries))
				{
					fprintf(stderr, "ERROR: Too many libraries specified\n");
					return 1;
				}
				strcat(libraries, argv[i]);
				if (i < argc - 1)
					strcat(libraries, " ");
			}
		}
	}
	// // Interactive mode if no arguments provided
	else
	{
		// 	printf("Filename: ");
		// 	if (fgets(file, sizeof(file), stdin) == NULL)
		// 	{
		// 		fprintf(stderr, "ERROR: Failed to read filename\n");
		// 		return 1;
		// 	}
		// 	file[strcspn(file, "\n")] = '\0';

		// 	// Validate that we have a filename
		if (strlen(file) == 0)
		{
			print_usage(argv[0]);
			return 1;
		}

		// 	printf("Libraries (e.g., readline ncurses) or press Enter for none: ");
		// 	if (fgets(libraries, sizeof(libraries), stdin) == NULL)
		// 	{
		// 		fprintf(stderr, "ERROR: Failed to read libraries\n");
		// 		return 1;
		// 	}
		// 	libraries[strcspn(libraries, "\n")] = '\0';
	}

	// if "file" has .c in it remove it
	int dot_index = strcspn(file, ".");
	if (dot_index != strlen(file))
	{
		file[dot_index] = '\0'; // add a break before .c
	}

	char output_path[256];
	if (snprintf(output_path, sizeof(output_path), "./bin/%s", file) >= sizeof(output_path))
	{
		fprintf(stderr, "ERROR: Path too long for output_path\n");
		return 1;
	}

	char file_path[256];
	if (snprintf(file_path, sizeof(file_path), "%s.c", file) >= sizeof(file_path))
	{
		fprintf(stderr, "ERROR: Path too long for file_path\n");
		return 1;
	}

	// Create output directory (if not exist)
	const char *folder = "bin";
	if (!nob_mkdir_if_not_exists(folder))
	{
		fprintf(stderr, "ERROR: Could not create %s directory\n", folder);
		return 1;
	}

	// Check if file exists
	if (!nob_file_exists(file_path))
	{
		fprintf(stderr, "ERROR: File '%s' does not exist\n", file_path);
		return 1;
	}

	// Setup compilation command
	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, "gcc");
	nob_cmd_append(&cmd, "-Wall");
	nob_cmd_append(&cmd, "-o");
	nob_cmd_append(&cmd, output_path);
	nob_cmd_append(&cmd, file_path);

	// Add library links if provided
	if (strlen(libraries) > 0)
	{
		char *lib = strtok(libraries, " ");
		while (lib != NULL)
		{
			char link_flag[256];
			snprintf(link_flag, sizeof(link_flag), "-l%s", lib);
			nob_cmd_append(&cmd, link_flag);
			lib = strtok(NULL, " ");
		}
	}

	// Run compilation
	if (!nob_cmd_run_sync(cmd))
	{
		fprintf(stderr, "ERROR: Compilation failed\n");
		return 1;
	}

	// Run the compiled program
	cmd.count = 0;
	nob_cmd_append(&cmd, output_path);
	if (!nob_cmd_run_sync(cmd))
	{
		fprintf(stderr, "ERROR: Program execution failed\n");
		return 1;
	}

	return 0;
}
