all: alice/alice bob/bob carol/carol freddie/freddie

args = -I. -g

alice/alice: alice/main.c bob/user.c bob/user.h carol/user.c carol/user.h freddie/user.c freddie/user.h
	clang alice/main.c bob/user.c carol/user.c freddie/user.c -o $@ $(args)

bob/user.c bob/user.h bob/server.c bob/server.h: bob/bob.defs
	mig -user bob/user.c -header bob/user.h \
		-server bob/server.c -sheader bob/server.h \
		bob/bob.defs

bob/bob: bob/main.c bob/server.c bob/server.h carol/user.c carol/user.h freddie/user.c freddie/user.h
	clang bob/main.c bob/server.c carol/user.c freddie/user.c -o $@ $(args)

carol/user.c carol/user.h carol/server.c carol/server.h: carol/carol.defs
	mig -user carol/user.c -header carol/user.h \
		-server carol/server.c -sheader carol/server.h \
		carol/carol.defs

carol/carol: carol/main.cpp carol/server.c carol/server.h
	clang++ carol/main.cpp carol/server.c -o $@ $(args)

freddie/user.c freddie/user.h freddie/server.c freddie/server.h: freddie/freddie.defs
	mig -user freddie/user.c -header freddie/user.h \
		-server freddie/server.c -sheader freddie/server.h \
		freddie/freddie.defs

freddie/freddie: freddie/main.cpp freddie/server.c freddie/server.h carol/user.h carol/user.c
	clang++ freddie/main.cpp freddie/server.c carol/user.c -o $@ $(args)

clear:
	rm -f alice/alice bob/bob bob/user.? bob/server.? \
		carol/carol carol/user.? carol/server.? \
		freddie/freddie freddie/user.? freddie/server.?
