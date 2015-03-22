#include <stdio.h>
#include "pbd/xml++.h"
#include "ardour/rc_configuration.h"

using namespace ARDOUR;
using namespace std;

int main (int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [-h] <file-name>\n", argv[0]);
		return -1;
	}

	if (!strcmp (argv[1], "-h") || !strcmp (argv[1], "--help")) {
		fprintf(stdout, "Usage: %s <file-name>\n\n", argv[0]);
		fprintf(stdout, "Writes the default Ardour config to the given file\n");
		return 0;
	}

	setenv ("ARDOUR_DLL_PATH", "/xxx", 1);
	setenv ("ARDOUR_CONFIG_PATH", "/xxx", 1);

	if (!ARDOUR::init (false, true, "/xxx")) {
		fprintf(stderr, "Failed to initialize libardour\n");
		return -1;
	}

	RCConfiguration * rc = new RCConfiguration;
	XMLNode& state = rc->get_state();

	XMLNode* cfg = state.child ("Config");
	cfg->remove_nodes_and_delete ("name", "donate-url");
	cfg->remove_nodes_and_delete ("name", "osx_pingback-url");
	cfg->remove_nodes_and_delete ("name", "linux-pingback-url");
	cfg->remove_nodes_and_delete ("name", "updates-url");
	cfg->remove_nodes_and_delete ("name", "freesound-download-dir"); // user specific

	XMLTree tree;
	tree.set_root (&state);

	if (!tree.write (argv[1])) {
		fprintf(stderr, "Error saving config file '%s'\n", argv[1]);
		return -1;
	}

	return 0;
}
