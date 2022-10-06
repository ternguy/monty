/*
bftxt : a brainfuck string generator.
Written by Francesco Palumbo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PRGNAME     "bftxt"
#define VERSION     "v0.1.1-r1"
#define AUTHOR      "Francesco Palumbo"
#define EMAIL       "phranz@subfc.net"

#define MULT        0x1
#define RELA        0x2
#define LOOP        0x4
#define COMM        0x8
#define NONL        0x10
#define ANLP        0x20

#define DIV         10

#define elif        else if


#define LEN(s, l)                       \
    for (l = 0; *(s + l); l++)          \


#define ZEROFILL(b, n)                  \
    do {                                \
        size_t i;                       \
        for (i = 0; i < n; b[i++] = 0); \
    } while (0)


#define BNPUTS(n, s)                    \
    do {                                \
        size_t i;                       \
                                        \
        for (i = 0; i < n; i++)         \
            pbputs(s);                  \
                                        \
    } while (0)


#define BPUTS(s)                        \
    pbputs(s)


#define BNPUTC(n, c)                    \
    pbputc(n, c)


#define BPUTC(c)                        \
    pbputc(1, c)


#define F_RELATIVE(c, n, l)             \
    do {                                \
        (n > l)                         \
            ? ((c = '+'), (n -= l))     \
            : ((c = '-'), (n = l - n))  \
            ;                           \
    } while (0)


#define PZEROLOOP()                     \
    BPUTS("[-]")                        \


#define PSTARTLOOP()                    \
    BPUTS("[>")                         \


#define PENDLOOP(n)                     \
    BNPUTS(n, "<-]")                    \


#define PNEXT(n)                        \
    BNPUTC(n, '>')                      \


#define PPREV(n)                        \
    BNPUTC(n, '<')                      \


#define PINC(n)                         \
    BNPUTC(n, '+')                      \
 
   
#define PDEC(n)                         \
    BNPUTC(n, '-')                      \
    

#define PWRITE()                        \
    BPUTC('.')


#define F_TEST_COMM_MULT(n)                             \
    do {                                                \
        if (flags & COMM) {                             \
                (n == '[')      ?   bputs(" |lb| ")     \
            :   (n == ']')      ?   bputs(" |rb| ")     \
            :   (n == '+')      ?   bputs(" |pl| ")     \
            :   (n == '-')      ?   bputs(" |mn| ")     \
            :   (n == '<')      ?   bputs(" |lt| ")     \
            :   (n == '>')      ?   bputs(" |gt| ")     \
            :   (n == '.')      ?   bputs(" |pt| ")     \
            :   (n == ',')      ?   bputs(" |cm| ")     \
            :   (n == ' ')      ?   bputs(" |sp| ")     \
            :   (n == '\n')     ?   bputs(" |nl| ")     \
            :   (n != '\0')     ?   (                   \
                                    bputs(" |"),        \
                                    bputc(1, n),        \
                                    bputs("| ")         \
                                    )                   \
                                :   0+0                 \
                                ;                       \
        }                                               \
                                                        \
        if (flags & MULT)                               \
            BPUTC('\n');                                \
                                                        \
    } while (0)


#define CONST_COMMON_INIT_CHECK_INLOOP();               \
    if (! *sptr)                                        \
        *sptr = '\n';                                   \
                                                        \
    if ((flags & NONL) && (*sptr == '\n'))              \
        break;                                          \
                                                        \
    cnum = *sptr;                                       \
                                                        \
    if (cnum == lastpos) {                              \
        PWRITE();                                       \
        F_TEST_COMM_MULT(*sptr);                        \
        continue;                                       \
    }


#define USAGE()                                                                     \
    puts(                                                                           \
        PRGNAME " " VERSION ", written by " AUTHOR " <" EMAIL "> \n"       \
        "Usage: " PRGNAME " [-h][-l][-a][-m][-r][-n][-c][-s <sep>] \"<string>\"\n"  \
        "\n"                                                                        \
        "  -h          print this help.\n"                                          \
        "  -l          use loops.\n"                                                \
        "  -a          use annidated loops.\n"                                      \
        "  -m          multiline output.\n"                                         \
        "  -r          generate by last char position.\n"                           \
        "  -n          suppress newline at the end of the string.\n"                \
        "  -c          add names of chars as comments.\n"                           \
        "  -s <sep>    use <sep> as separator between chars.\n"                     \
        "\n"                                                                        \
        "This program is free software: you can redistribute it and/or modify\n"    \
        "it under the terms of the GNU General Public License as published by\n"    \
        "the Free Software Foundation, either version 3 of the License, or\n"       \
        "(at your option) any later version.\n"                                     \
        "\n"                                                                        \
        "This program is distributed in the hope that it will be useful,\n"         \
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"          \
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"           \
        "GNU General Public License for more details.\n"                            \
        "\n"                                                                        \
        "You should have received a copy of the GNU General Public License\n"       \
        "along with this program. If not, see <http://www.gnu.org/licenses/>.")     \



void bputs          (const char *);
void bsputs         (const char *);
void bputc          (unsigned int, char);
void bsputc         (unsigned int, char);
void bfproc         (char *);

void proc_rel       (char *);
void proc_def       (char *);

void (* pbputs)     (const char *);
void (* pbputc)     (unsigned int, char);

size_t check_sep    (char);
size_t maxl         (const char *);
size_t factors      (unsigned int *, int, int);

inline size_t hae   (int);

char   sep   = 0;

size_t flags = 0;
size_t lb    = 0;


size_t hae(int val)
{
    USAGE();
    exit(val);
}


size_t check_sep(char s)
{
    char bf_chars[] = "+-.,[]><";   
    size_t i = 0;                   
                                    
    while (bf_chars[i])             
        if (bf_chars[i++] == s)     
            return 0;

    return 1;
}


void bputs(const char *s)
{
    fputs(s, stdout);
}


void bsputs(const char *s)
{    
    size_t i, l;
    LEN(s, l);

    for (i = 0; i < l; i++) {
        putchar(*(s + i));
        putchar(sep);
    }
}


void bputc(unsigned int n, char c)
{
    size_t i;
  
    if (!n)
        return;

    if (n == 1)
        putchar(c);
    else
        for (i = 0; i < n; i++)
            putchar(c);
}


void bsputc(unsigned int n, char c)
{
    size_t i;
  
    if (!n) 
        return;

    if (n == 1) {
        putchar(c);
        putchar(sep);
    }
    else {
        for (i = 0; i < n; i++) {
            putchar(c);
            putchar(sep);
        }
    }
}


size_t factors(unsigned int *arr, int s, int m)
{
    static int i;

    if (!m)
        return (i = 0);

    if (m == 1) {
        if (i + 1 < s) arr[i + 1] = 0;
        return i ? i : 1;
    }

    if (arr != NULL) {
        for (; (i < s) && m; i++) {
            if (! (m % 2))
                m /= (arr[i] = 2);
            elif (! (m % 3))
                m /= (arr[i] = 3);
            else {
                (m == 1)
                    ?  i--
                    :  (arr[i] = m)
                    ;

                m = 0;
            }
        }

        arr[i] = 0;
    }
    else {
        for (i = 0; (i < s) && m; i++) {
            if (! (m % 2))
                m /= 2;
            elif (! (m % 3))
                m /= 3;
            else {
                if (m == 1) 
                    i--;

                m = 0;
            }
        }
    }

    return i;
}


size_t maxl(const char *string)
{
    size_t i;
    size_t nmax = 1;
    size_t elnum;

    for (i = 0; *(string + i); i++) {
        elnum  = factors(NULL, DIV, DIV);
        elnum += factors(NULL, DIV, *(string + i) / DIV);

        if (elnum > nmax)
            nmax = elnum;

        factors(NULL, 0, 0);
    }

    return nmax;
}


void proc_rel(char *sptr)
{
    unsigned int mul;
    unsigned int res;
    unsigned int cnum;

    size_t lastpos = 0;

    int alevel = 0;

    char c = '+';

    if (flags & LOOP) {
        do {
            CONST_COMMON_INIT_CHECK_INLOOP();
            F_RELATIVE(c, cnum, lastpos);

            if (cnum > DIV) {
                if (lastpos)
                    PPREV(1);

                mul = cnum / DIV;
                res = cnum % DIV;

                PINC(mul);
                PSTARTLOOP();
                BNPUTC(DIV, c);
                PENDLOOP(1);
                PNEXT(1);
            } 
            else 
                res = cnum;
         
            BNPUTC(res, c);
            PWRITE();

            F_TEST_COMM_MULT(*sptr);

        } while ((lastpos = *(sptr++)) != '\n');
    }
    elif (flags & ANLP) {
        int maxdn  = 1;
        int i      = 1;
        unsigned int mularr[DIV];

        maxdn = maxl(sptr);

        do {
            CONST_COMMON_INIT_CHECK_INLOOP();
            F_RELATIVE(c, cnum, lastpos);

            if (cnum > DIV) {
                mul = cnum / DIV;
                res = cnum % DIV;

                alevel = factors(mularr, DIV, DIV);
                alevel = factors(mularr, DIV, mul);
            }
            else {
                alevel = factors(mularr, DIV, cnum);
                res = 0;
            }

            (!lastpos)  ?   PNEXT((maxdn - alevel) > 0 ? maxdn - alevel : 0) 
                        :   PPREV(alevel - 1);

            for (i = 0; i < alevel; i++) {
                    (alevel == 1)           ?   BNPUTC(cnum, c)
                :   (i == (alevel - 1))     ?   BNPUTC(mularr[i], c)
                                            :   PINC(mularr[i])
                                            ;

                if (alevel - 1 - i > 0) PSTARTLOOP();
            }

            PENDLOOP(alevel - 1);
            PNEXT(alevel - 1);
            BNPUTC(res, c);
            PWRITE();

            factors(mularr, 0, 0);

            F_TEST_COMM_MULT(*sptr);

        } while ((lastpos = *(sptr++)) != '\n');
    }
    else {
        do {
            CONST_COMMON_INIT_CHECK_INLOOP();
            F_RELATIVE(c, cnum, lastpos);

            BNPUTC(cnum, c);
            PWRITE();
            F_TEST_COMM_MULT(*sptr);

        } while ((lastpos = *(sptr++)) != '\n');
    }
}


void proc_def(char *sptr)
{
    unsigned int mul;
    unsigned int res;
    unsigned int cnum;

    size_t lastpos = 0;

    int alevel = 0;

    if (flags & LOOP) {
        do {
            CONST_COMMON_INIT_CHECK_INLOOP();

            if (lastpos) {
                PZEROLOOP();
                PPREV(1);
            }

            if (cnum > DIV) {
                if (lastpos)
                    PPREV(1);

                mul = cnum / DIV;
                res = cnum % DIV;

                PINC(mul);
                PSTARTLOOP();
                PINC(DIV);
                PENDLOOP(1);
                PNEXT(1);
            } 
            else 
                res = cnum;
         
            PINC(res);
            PWRITE();

            F_TEST_COMM_MULT(*sptr);

        } while ((lastpos = *(sptr++)) != '\n');
    }
    elif (flags & ANLP) {
        do {
            int i = 1;
            unsigned int mularr[DIV];

            CONST_COMMON_INIT_CHECK_INLOOP();

            if (lastpos) {
                PZEROLOOP(); 
                PPREV(alevel - 1);
            }

            mul = cnum / DIV;
            res = cnum % DIV;
            
            alevel  = factors(mularr, DIV, DIV) - 1;
            alevel += factors(mularr, DIV, mul) - 1;

            for (i = 0; i < alevel; i++) {
                PINC(mularr[i]);

                if (i < alevel - 1)
                    PSTARTLOOP();
            }

            PENDLOOP(alevel - 1);
            PNEXT(alevel - 1);
            PINC(res);
            PWRITE();

            factors(mularr, 0, 0);

            F_TEST_COMM_MULT(*sptr);

        } while ((lastpos = *(sptr++)) != '\n');
    }
    else {
        do {
            CONST_COMMON_INIT_CHECK_INLOOP();
            PDEC(lastpos);

            PINC(cnum);
            PWRITE();
            F_TEST_COMM_MULT(*sptr);

        } while ((lastpos = *(sptr++)) != '\n');
    }
}


void bfproc(char *sptr) 
{
    if (sep) {
        pbputs = bsputs;
        pbputc = bsputc;
    }

    (flags & RELA)
        ?   proc_rel(sptr)
        :   proc_def(sptr)
        ;

    bputc(1, '\n');
}


int main(int argc, char *argv[])
{
    char opt;

    if (argc == 1)
        hae(EXIT_FAILURE);

    pbputs  = bputs;
    pbputc  = bputc;

    while ((opt = getopt(argc, argv, "hlamrncs:")) != -1) {

            (opt == 'h')    ?   hae(EXIT_SUCCESS)

        :   (opt == 'l')    ?   (flags |= LOOP)
        :   (opt == 'a')    ?   (flags |= ANLP)
        :   (opt == 'm')    ?   (flags |= MULT)
        :   (opt == 'r')    ?   (flags |= RELA)
        :   (opt == 'n')    ?   (flags |= NONL)
        :   (opt == 'c')    ?   (flags |= COMM)

        :   (opt == 's')    ?   (check_sep(
                                 sep = *optarg) || 
                                hae(EXIT_FAILURE))

                            :   hae(EXIT_FAILURE)
                            ;
    }

    if (argv[optind] == NULL)
        hae(EXIT_FAILURE);

    flags ^= (flags & ANLP) && (flags & LOOP) ? LOOP : 0;
    (flags & MULT) ? setvbuf(stdout, NULL, _IOFBF, BUFSIZ) : 0+0;

    bfproc(argv[optind]);

    return 0;
}
