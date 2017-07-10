#include <stdlib.h>
#include <error.h>
#include <argp.h>

static char doc[] = "A command line tool to use Posix Message Queues from the shell"
					"\vCommands:\n"
					"  create    Create a POSIX message queue\n"
					"  info      Print information about an existing message queue\n"
					"  unlink    Delete a message queue\n"
					"  send      Send a message to a message queue\n"
					"  recv      Receive and print a message from a message queue\n"
					 ;
static char args_doc[] = "COMMAND [ MESSAGE ]";

void usage()
{
	printf("usage: mq <command> <args>\n"
		   "\n"
		   "mq create <q-name> [-m maxmsg] [-s msgsize]\n"
		   "mq info <q-name>\n"
		   "mq unlink <q-name>\n"
		   "mq recv <q-name> [-n]\n"
		   "mq send <q-name> [message]\n"
		   "\n"
		  );
}

static struct argp_option options[] = {
	{ "verbose", 'v', 0, 0, "Produce verbose output" },
	{ 0 }
};

struct arguments
{
	int verbose;
	char *command;
	char *qname; /* name of the mq (should start with '/') */

	/* for command 'create' */
	int maxmsg; /* max number of message */
	int msgsize; /* size of a message */
	int force; /* do not return an error if queue already exists */

	/* for command 'recv' */
	int blocking;

	/* for command 'send' */
	char *message;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	 * know is a pointer to our arguments structure. */
	struct arguments *args = state->input;

	switch (key) {
	case 'v':
	  args->verbose = 1;
	  break;

	case ARGP_KEY_NO_ARGS:
	  argp_usage(state);

	case ARGP_KEY_ARG:
		/* Here we know that state->arg_num == 0, since we
		   force argument parsing to end before any more arguments can
		   get here. */
		args->command = arg;

		/* Now we consume all the rest of the arguments.
		   state->next is the index in state->argv of the
		   next argument to be parsed, which is the first string
		   we’re interested in, so we can just use
		   &state->argv[state->next] as the value for
		   arguments->strings.

		   In addition, by setting state->next to the end
		   of the arguments, we can force argp to stop parsing here and
		   return. */
		args->message = state->argv[state->next];
		state->next = state->argc;

		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}


int main(int argc, char **argv)
{
	struct arguments args;
	struct argp argp = { options, parse_opt, args_doc, doc };

	/* Default values */
	args.verbose = 0;
	args.command = NULL;
	args.qname = NULL;
	args.maxmsg = 10;
	args.msgsize = 1024;
	args.force = 0;
	args.blocking = 0;
	args.message = NULL;

	argp_parse(&argp, argc, argv, 0, 0, &args);

}
