LDFLAGS+= # Les libs supplementaires
CFLAGS+= -Werror -Wextra -Wall -Wno-error=unused-parameter
DEPS=provided.h enonce.c
OBJS=prolog.c provided.o

# Si besoin d'exclure des noms de fonctions :
# ILLEGAL= # Une expression régulière compatible egrep
#          # interdisant certains noms de fonctions

# Si besoin d'un prolog contenant le fichier copy.c :
#
# DEPS=prolog.c provided.h enonce.c
# OBJS=provided.o copy.o prolog.c
# prolog.c: copy.c
#         ../lib/extract_subject copy.c > $@

# Si besoin d'un vérificateur de mémoire :
#
# MALLOC=sentinel
