def makeAlice(carol, freddie) {
    def channel {}
    def carolClient := carol.checkIn()

    def alice {
        to proveTo(bob) :void {
            def passport := carolClient.makePassport()
            def handle := freddie.makeHandle(passport, channel)
            bob.prove(handle)
        }
    }
    return alice
}

def makeBob(carol, freddie) {
    def bob {
        to prove(handle) :void {
            def [passport, channel] := freddie.readHandle(handle)
            def id := carol.readPassport(passport)
            println(`Bob: ID is $id, channel is $channel`)
        }
    }
    return bob
}

def makeCarol() {
    var id := 0
    def passportToId := [].asMap().diverge()
    def carol {
        to checkIn() {
            def myId := id
            id += 1
            def client {
                to getId() :int {
                    return myId
                }
                to makePassport() :any {
                    var sealed := false
                    def passport {
                        to seal() :void {
                            if (sealed) {
                                throw <- "Already sealed"
                            }
                            sealed := true
                        }
                    }
                    passportToId[passport] := myId
                    return passport
                }
            }
            return client
        }
        to readPassport(passport) :int {
            return passportToId[passport]
        }
    }
    return carol
}

def makeFreddie() {
    def passports := [].asMap().diverge()
    def channels := [].asMap().diverge()

    def freddie {
        to makeHandle(passport, channel) :any {
            passport.seal()
            def handle {}
            passports[handle] := passport
            channels[handle] := channel
            return handle
        }

        to readHandle(handle) {
            return [passports[handle], channels[handle]]
        }
    }
    return freddie
}


def carol := makeCarol()
def freddie := makeFreddie()
def alice := makeAlice(carol, freddie)
def bob := makeBob(carol, freddie)
alice.proveTo(bob)
