class Main {
    private static void good() {
        System.out.println("Good:");
        Carol carol = new Carol();
        Freddie freddie = new Freddie();
        Alice alice = new Alice(carol, freddie);
        Bob bob = new Bob(carol, freddie);
        alice.proveTo(bob);
    }

    private static void differentFreddie() {
        System.out.println("Different Freddie:");
        Carol carol = new Carol();
        Alice alice = new Alice(carol, new Freddie());
        Bob bob = new Bob(carol, new Freddie());
        try {
            alice.proveTo(bob);
        } catch (RuntimeException ex) {
            ex.printStackTrace();
        }
    }

    private static void differentCarol() {
        System.out.println("Different Carol:");
        Freddie freddie = new Freddie();
        Alice alice = new Alice(new Carol(), freddie);
        Bob bob = new Bob(new Carol(), freddie);
        try {
            alice.proveTo(bob);
        } catch (RuntimeException ex) {
            ex.printStackTrace();
        }
    }

    private static void eveInTheMiddlePassthough() {
        System.out.println("Eve-in-the-middle passthrough:");
        Carol carol = new Carol();
        Freddie freddie = new Freddie();
        Alice alice = new Alice(carol, freddie);
        Bob bob = new Bob(carol, freddie);
        Eve eve = new Eve(Eve.Behavior.PASSTHROUGH, bob, freddie);
        try {
            alice.proveTo(eve);
        } catch (RuntimeException ex) {
            ex.printStackTrace();
        }
    }

    private static void eveInTheMiddleMakeNewHandle() {
        System.out.println("Eve-in-the-middle making new handle:");
        Carol carol = new Carol();
        Freddie freddie = new Freddie();
        Alice alice = new Alice(carol, freddie);
        Bob bob = new Bob(carol, freddie);
        Eve eve = new Eve(Eve.Behavior.MAKE_NEW_HANDLE, bob, freddie);
        try {
            alice.proveTo(eve);
        } catch (RuntimeException ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String[] args) {
        // Works
        good();
        System.out.println();

        // Throws Freddie.InvalidHandleException
        differentFreddie();
        System.out.println();

        // Throws Carol.InvalidPassportException
        differentCarol();
        System.out.println();

        // Works, but Bob still gets Alice's channel
        eveInTheMiddlePassthough();
        System.out.println();

        // Throws Carol.Passport.AlreadySealedException
        eveInTheMiddleMakeNewHandle();
    }
}
