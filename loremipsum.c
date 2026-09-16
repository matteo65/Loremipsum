/*
 * loremipsum.c
 *
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Compile: gcc -O3 loremipsum.c -o loremipsum
 *
 * Sintax: loremipsum [[start] [options]]
 *    Options:
 *        -newline            Write a newline after every sentence (default no newline)
 *        -seed <number>      Initialize the random generator to the seed (default seed=random)
 *        -sentences <number> Write <number> sentences (default number = infinite)
 *        -counter            Write a sequential number at the beginning of the sentences 
 *
 *    No args prints help
 *
 * Command line "Lorem Ipsum" text generator.
 * Writes random Lorem Ipsum style ASCII text to stdout.
 * The text contains only uppercase and lowercase alphabetic characters, the period, the comma, and the exclamation mark.
 * Each sentence may begin with a sequential number (counter).
 * Uses 48 randomly chosen predefined sentences.
 *
 */

#include <stdio.h>
#include <string.h> // strcmp()
#include <stdint.h> // uint64_t
#include <inttypes.h> // PRIu64
#include <time.h> // clock_gettime()
#include <stdlib.h> // strtoull()
#include <errno.h> // errno
#include <ctype.h> // isdigit()

static const char *sentences[] = {
	"At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga.",
	"Autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.",
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.",
	"Et harum quidem rerum facilis est et expedita distinctio.",
	"Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat.",
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed diam nonnumy eiusmod tempor incidunt ut labore et dolore magna aliquam erat voluptat.",
	"Ut enim ad minim veniam, quis nostrud exercitation ullamcorpor suscipit laborios nisi ut aliquip ex ea commodo consequat.",
	"Duis autem vel eum irure reprehenderit in voluptate velit esse nihil molestiae consequat, vel illum dolore eu fugiat nulla pariatur.",
	"At vero eos et accusam et iusto odio dignissim ducim qui blandit praesent luptatum delenit atgue duos dolor et molestias exceptur sint occaecat cupiditat non provident, simil sunt in culpa qui officia deserunt mollit anim id est laborum et dolor fuga.",
	"Et harumd dererud facilis est er expedit distinct.",
	"Nam liber tempor cum soluta nobis est eligend optio comgue nihil impedit doming id quod maxim placeat facer possim omnis voluptas assumenda est, omnis dolor repellend.",
	"Temporibud autem quinusd et aur office debit aut rerum necessit atib saepe eveniet ut er repudiand sint et molestia non recusand.",
	"Itaque earud rerum hic tenetury sapiente delectus ut aut prefer endis dolorib asperiore repellat.",
	"Hanc ego cum tene sententiam, quid est cur verear ne ad eam non possing accommodare torquat nost ros quos tu paulo ante cum memorite tum etia ergat.",
	"Nos amice et nebevol molestias access potest fier ad augendas cum conscient to factor tum poen legum odioque civiuda.",
	"Et tamen in busdam neque pecun modut est neque nonor imper ned libiding gen epular religuard cupiditat, quas nulla praid on umdant.",
	"Improb pary minuit, potius inflammad ut coercend magist and dedocendesse videantur.",
	"Invitat igitur vera ratio bene sanos ad iustitiam, aequitated fidem.",
	"Neque hominy infant aut iniuste fact cond qui neg facile efficerd possit duo conetud notiner si effecerit, et opes vel fortunag vel ingen liberalitat magis conveniunt, da but tuntug benevolent sib conciliant et, aptissim est ad quiet.",
	"Endium caritat praesert cum omning null sit caus peccand quaet en imigent cupidtat a natura proficis facile explent sine ulla inura autend inane sunt is parend non est nihil enim desiderabile.",
	"Concupis plusque in ipsinuria detriment est quam in his rebus emolument pariunt iniur.",
	"Itaque ne iustitial dem rect quis dixer per se ipsad optabil, sed quiran cunditat vel plurify afferat.",
	"Nam dilig et carum esse iucund est propter tutior vitam et luptat pleniore efficit.",
	"Tia non ob ea solu incommod quae egenium improb fugiendad improbitate putamuy sed mult etiam mag quod cuis.",
	"Guae ad amicos pertineren garent esse per se sas tam expetend quam nostras expetere quo loco videtur quibusing stalibilit amicitiae acillard tuent tamet eum locum seque facil, ut mihi detur expedium.",
	"It enim virtutes, de quib ante dictum est, sic amicitiand neg posse a luptate discedere.",
	"Nam cum solitud et vitary sing amicis insidar et metus plena sit, ratiodipsa monet amicitian comparar, quibus part confirmatur animuset a spe pariendar luptam seiung non poest. Atque ut odia, invid despication adversantur luptatib, sic amicitian non modo fautrices fidelissim sed etiam effectrice sunt luptam amic quam busnon.",
	"Praesentib fruunt sed etiam spe erigunt consequent ac poster tempor most es muy fuerte. Null modo sine amicit firma et perpetuam incandit vitae tenere possum neque ero psam tuer nisi amicos et nosmet diligam idcirco et boctor ipsumed effit in amicitad cum oluptation.",
	"Mam et laetam amico consid aeque nostralet parit dolem angorib.",
	"Suocirca eodem mod sapiens erit affect erg amicund quo in perseus, duos labor propter suam susciper Euae de virtutib dictal guemad modum eae sempan inhaerercent, eadem de amicit dicend.",
	"Praeclar enim picur his paen verbis adem inquite senten confirmavit anim ne aut sempitern aut diuturning timer naturan it salar le grand.",
	"Auam multa vitiose and non distrib et indicat con ingenion.",
	"Sumde enim bon et malut puer utra dicabit, dysa stante an movente propter fegum ailman.",
	"Buoniam si dis placet ab picuro loqui discim si sante hoc naturing delicet pullam salvan esse concede mus si movent.",
	"Tamen dicitis nulla turnen luptas erit praedermit sit et simul non proficiscit animal illud modo natum a sunma est a te post in mon dolend.",
	"Nectamen argument hoc picurus a parvis petivit aut etiam a bestis, uae putat esse specula naturae, ut diceret abbis duce hanc expetion bihil dolend.",
	"Ectamen nedue enim haec movere potest appetit anim ned ullam habet ictum pellat peccage eronylar at ille pellit sensar luptae epicur semper hoc ut provert povultan.",
	"For natura expeting ea in motuon sit et parvos ad se alliciat et stiady non illa stabil in tant in estnian doler, non solud in indutial genelation.",
	"What gitur convente ab alia dicer naturam proficis in alia summon bon ponere, es un peculiat of harmanion.",
	"Pero nullion diciud putialenim depra et non.",
	"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum.",
	"Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat, facere possimus, omnis voluptas assumenda est, omnis dolor repellendus.",
	"Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit, amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt, ut labore et dolore magnam aliquam quaerat voluptatem.",
	"Quis autem vel eum iure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat quo voluptas nulla pariatur? ",
	"Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo.",
	"Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae.",
	"Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? ",
	"Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat."
};

void usage(void);
void invalid_arguments(const char *arg);
int is_valid_uint64(const char *str, uint64_t *out_value);
uint64_t random_from_clock(void);
uint64_t sirius64(uint64_t *state);
size_t get_random(void);

static int opt_newline = 0;
static int opt_counter = 0;
static uint64_t seed;

int main(int argc, const char *argv[])
{
	uint64_t nsentences = UINT64_MAX;

	if(argc <= 1) {
		usage();
		return 1;
	}

	if(strcmp(argv[1], "start") != 0) {
		invalid_arguments(argv[1]);
		return 1;
	}

	seed = random_from_clock();

	for(int i = 2; i < argc; i++) {
		if(strcmp(argv[i], "-newline") == 0) {
			opt_newline = 1;
		}
		else if(strcmp(argv[i], "-counter") == 0) {
			opt_counter = 1;
		}
		else if(strcmp(argv[i], "-seed") == 0) {
			if(i >= argc - 1 || !is_valid_uint64(argv[++i], &seed)) {
				invalid_arguments(argv[i]);
				return 1;
			}
		}
		else if(strcmp(argv[i], "-sentences") == 0) {
			if(i >= argc - 1 || !is_valid_uint64(argv[++i], &nsentences)) {
				invalid_arguments(argv[i]);
				return 1;
			}
		}
		else {
			invalid_arguments(argv[i]);
			return 1;
		}
	}
	if(nsentences == 0) {
		return 0;
	}
	
	if(opt_counter) {
		printf("1 ");
	}
	printf("%s", sentences[get_random()]);
	
	for(uint64_t i = 2; i <= nsentences; i++) {
		if(opt_newline) {
			printf("\n");
		}
		else {
			printf(" ");
		}
		if(opt_counter) {
			printf("%" PRIu64 " ", i);
		}

		printf("%s", sentences[get_random()]);
	}
	if(opt_newline) {
		printf("\n");
	}
	return 0;
}

void usage(void)
{
	printf("loremipsum - Copyright(C) Matteo Zapparoli\n");
	printf("Writes random Lorem Ipsum style ASCII text to stdout\n");
	printf("The text contains only uppercase and lowercase alphabetic characters, the period, the comma, and the exclamation mark\n");
	printf("Uses %zu randomly chosen predefined sentences\n", (sizeof(sentences) / sizeof(char*)));
	printf("Sintax: loremipsum [[start] [options]]\n\n");
	printf("Options:\n");
	printf("-newline            Write a newline after every sentence (default no newline)\n");
	printf("-seed <number>      Initialize the random generator to the seed (default seed = random)\n");
	printf("-sentences <number> Write <number> sentences (default number = infinite)\n");
	printf("-counter            Write a sequential number at the beginning of the sentences\n\n");
	printf("No args prints help\n");
}

void invalid_arguments(const char *arg)
{
	fprintf(stderr, "*** Error: invalid arguments %s\n", arg);
}

// Return 1 id str a uint64_t, 0 else
int is_valid_uint64(const char *str, uint64_t *out_value)
{
	if (str == NULL || *str == '\0') return 0;

	// Controlla che la stringa contenga solo cifre
	for (const char *p = str; *p; p++) {
		if (!isdigit((unsigned char)*p)) return 0;
	}

	errno = 0;
	char *end;
	unsigned long long v = strtoull(str, &end, 10);

	// end non deve spostarsi oltre la fine
	if (*end != '\0') return 0;

	// Overflow rilevato da errno
	if (errno == ERANGE) return 0;

	// Controllo ulteriore: strtoull potrebbe essere > UINT64_MAX su alcune piattaforme
	if (v > UINT64_MAX) return 0;

	if (out_value) *out_value = (uint64_t)v;

	return 1;
}

// Return a simple "pseudo-random" based on clock
uint64_t random_from_clock(void)
{
	static uint64_t counter = 0;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	uint64_t x = ((uint64_t)ts.tv_sec << 32) ^ (uint64_t)ts.tv_nsec;

	// aggiungi un contatore per garantire unicità
	x ^= (++counter * 0x9E3779B97F4A7C15ULL);

	// mixing forte (stile splitmix64)
	x ^= x >> 33;
	x *= 0xff51afd7ed558ccdULL;
	x ^= x >> 33;
	x *= 0xc4ceb9fe1a85ec53ULL;
	x ^= x >> 33;

	return x;
}

// Sirius64 PRNG: https://github.com/matteo65/Sirius64
uint64_t sirius64(uint64_t *state)
{
	uint64_t z = (*state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * ((*state) ^ z ^ (z >> 17));
}

static const uint64_t limit = UINT64_MAX - (UINT64_MAX % (sizeof(sentences) / sizeof(char*)));

size_t get_random(void)
{
    uint64_t rand_num;
    do {
        rand_num = sirius64(&seed);
    } 
	while (rand_num >= limit);

    return rand_num % (sizeof(sentences) / sizeof(char*));
}
