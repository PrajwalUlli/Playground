#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	// Build itself
	NOB_GO_REBUILD_URSELF(argc, argv);

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
	nob_cmd_append(&cmd, "<output_path>");
	nob_cmd_append(&cmd, "<file_path>");
  nob_cmd_append(&cmd, "-l<library>");

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
