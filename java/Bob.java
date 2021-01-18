interface BobInterface {
    void prove(Freddie.Handle handle) throws Freddie.InvalidHandleException, Carol.InvalidPassportException;
}

class Bob implements BobInterface {
    private Carol carol;
    private Freddie freddie;

    Bob(Carol carol, Freddie freddie) {
        this.carol = carol;
        this.freddie = freddie;
    }

    public void prove(Freddie.Handle handle) throws Freddie.InvalidHandleException, Carol.InvalidPassportException {
        freddie.verifyHandle(handle);
        Carol.Passport passport = handle.getPassport();
        carol.verifyPassport(passport);
        int id = passport.getClientId();
        System.out.format("Bob: ID is %d, channel is %s\n", id, handle.getChannel());
    }
}
