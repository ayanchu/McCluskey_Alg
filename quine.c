 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <c++/v1/cstdlib>

 typedef char qc_t;

static const qc_t TRUE  =    1;
static const qc_t FALSE =    0;
static const qc_t DONTCARE = 2;

#ifdef DEBUG
#define DEBUG_ENTRY printf("Entering %s/%s: %i\n", __FILE__, __FUNCTION__, __LINE__)
#define DEBUG_EXIT printf("Exiting %s/%s: %i\n", __FILE__, __FUNCTION__, __LINE__)
#define DEBUG_PASS printf("Pass at %s/%s: %i\n", __FILE__, __FUNCTION__, __LINE__)
#else
#define DEBUG_ENTRY
#define DEBUG_EXIT
#define DEBUG_PASS
#endif


/**
 * Will combine two terms into one, resulting in a new term in case both can be combined, NULL
 * otherwise.
 */
const qc_t *reduce(const qc_t *term1, const qc_t *term2, const size_t length) {
   	int i;
   	int pos = -1;
    qc_t *newterm;

    DEBUG_ENTRY;

    // If pointers are equal, it's the same
    if (term1 == term2) {
    	return NULL;
    }

	newterm = (qc_t *) malloc(sizeof(qc_t) * length);

   	for (i = 0; i < length; i++) {
   		if (term1[i] != term2[i]) {
			if (pos != -1) {

   				return NULL;
   			}
   			pos = i;
   			newterm[i] = DONTCARE;
   		} else {
   			newterm[i] = term1[i];
   		}
   	}

   	if (pos == -1) {

   		free(newterm);
   		return NULL;
   	}

   	return newterm;
}

const qc_t **reduceall(const qc_t **table, int nterms, int length) {
	int i, j, pos;

	qc_t const **newtable = (qc_t const **) malloc(sizeof(void *) * nterms);

	DEBUG_ENTRY;
	memset(newtable, 0, nterms);

	pos = 0;
	for (i = 0; i < nterms; i++) {
		for (j = 0; j < nterms; j++) {
			if (i == j) {
				continue;
			}

			const qc_t *reduced = reduce(table[i], table[j], length);
			if (reduced != NULL) {
				newtable[pos++] = reduced;
			}
		}
	}
	DEBUG_EXIT;

	return newtable;
}

void printterm(const qc_t *term, size_t length) {
	int i;

	DEBUG_ENTRY;

	if (term == NULL) {
		printf("unreducible");
	} else {
		for (i = 0; i < length; i++) {
			if (term[i] == DONTCARE) {
				printf("_");
			} else {
				printf("%i", term[i]);
			}
		}
	}

	DEBUG_EXIT;
}

void printtable(const qc_t **table, const int nterms, const int length) {
	int i;

	DEBUG_ENTRY;

	if (table == NULL) {
		DEBUG_PASS;
		return;
	}

	DEBUG_PASS;

	for (i = 0; i < nterms; i++) {
		DEBUG_PASS;
		if (table[i] == NULL) {
			DEBUG_PASS;
			continue;
		}

		DEBUG_PASS;
		printterm(table[i], length);

		DEBUG_PASS;
		printf("\n");
	}
	DEBUG_EXIT;
}

void copyterm(const qc_t *const cterm, qc_t *term, int length) {
	int i;
	for (i = 0; i < length; i++) {
		term[i] = cterm[i];
	}
}

void inc(qc_t *term, int len) {
	for (; len >= 0; --len) {
		if (term[len] == 0) {
			term[len] = 1;
			return;
		} else {
			term[len] = 0;
		}
	}
}

struct parse_output {
	const qc_t **table;
	int terms;
	int vars;
};

const struct parse_output *parse(const char *values) {
	int ln2, lnv, i, vallen, pos = 0, nterms = 0;
	qc_t const **table;
	qc_t *cterm;

	vallen = lnv = strlen(values);

	for (ln2 = 0; lnv > 1; ln2++, lnv /= 2);

	for (i = 0; i < vallen; i++) {
		if (values[i] == '1') {
			nterms++;
		}
	}

	cterm = (qc_t *) malloc(sizeof(qc_t) * nterms);
	for (i = 0; i < nterms; i++) {
		cterm[i] = 0;
	}

	table = (qc_t const **) malloc(sizeof(qc_t *) * nterms);


	for (i = 0; i < vallen; i++) {
		if (values[i] == '1') {
			qc_t *term = (qc_t *) malloc(sizeof(qc_t) * ln2);
			copyterm(cterm, term, ln2);
			table[pos++] = term;
			inc(cterm, ln2);
		}
	}

	struct parse_output *output = (struct parse_output *) malloc(sizeof(struct parse_output));
	output->table = table;
	output->terms = nterms;
	output->vars = ln2;

	return output;
}


// #ifdef USE_C_MAIN
//
// int main(int argc, char **argv) {
// 	if (argc < 2) {
// 		return 1; // Return a non-zero value to indicate an error
// 	}
//
// 	const struct parse_output *parse_output = parse(argv[1]);
//
// 	printtable(parse_output->table, parse_output->terms, parse_output->vars);
//
// 	system("PAUSE"); // This may not be portable; consider using getchar() for cross-platform compatibility
// 	return 0; // Return 0 to indicate successful execution
// }
// #endif
