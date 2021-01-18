class Alice {
    private Carol.Client carolClient;
    private Freddie freddie;
    private Object channel;

    Alice(Carol carol, Freddie freddie) {
        this.carolClient = carol.checkIn();
        this.freddie = freddie;
        this.channel = new Object();
        System.out.format("Alice: my channel is %s\n", channel);
    }

    void proveTo(BobInterface bob) {
        Carol.Passport passport = carolClient.makePassport();
        Freddie.Handle handle;
        try {
            handle = freddie.makeHandle(passport, channel);
        } catch (Carol.Passport.AlreadySealedException ex) {
            throw new RuntimeException("A freshly made passport is already sealed!", ex);
        }
        try {
            bob.prove(handle);
        } catch (Freddie.InvalidHandleException ex) {
            throw new RuntimeException("Freddie doesn't like the handle", ex);
        } catch (Carol.InvalidPassportException ex) {
            throw new RuntimeException("Carol doesn't like the passport", ex);
        }
    }
}
