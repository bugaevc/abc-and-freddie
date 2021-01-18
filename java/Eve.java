class Eve implements BobInterface {
    private Behavior behavior;
    private BobInterface bob;
    private Freddie freddie;
    private Object channel;

    enum Behavior {
        PASSTHROUGH,
        MAKE_NEW_HANDLE,
    }

    Eve(Behavior behavior, BobInterface bob, Freddie freddie) {
        this.behavior = behavior;
        this.bob = bob;
        this.freddie = freddie;
        this.channel = new Object();
        System.out.format("Eve: my channel is %s\n", channel);
    }

    public void prove(Freddie.Handle handle) throws Freddie.InvalidHandleException, Carol.InvalidPassportException {
        switch (behavior) {

        case PASSTHROUGH:
            bob.prove(handle);
            break;

        case MAKE_NEW_HANDLE:
            Carol.Passport passport = handle.getPassport();
            try {
                handle = freddie.makeHandle(passport, channel);
            } catch (Carol.Passport.AlreadySealedException ex) {
                throw new RuntimeException("Oh no, exploit failed!", ex);
            }
            bob.prove(handle);
            break;
        }
    }
}
