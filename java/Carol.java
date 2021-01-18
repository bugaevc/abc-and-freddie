import java.util.HashSet;
import java.util.Set;

class Carol {
    private int nextClientId = 0;
    private Set<Passport> passports = new HashSet<>();

    class Client {
        private int id;

        Client(int id) {
            this.id = id;
        }

        Passport makePassport() {
            Passport passport = new Passport(this);
            passports.add(passport);
            return passport;
        }
    }

    static class Passport {
        private Client client;
        private boolean sealed = false;

        Passport(Client client) {
            this.client = client;
        }

        void seal() throws AlreadySealedException {
            if (sealed) {
                throw new AlreadySealedException();
            }
            sealed = true;
        }

        int getClientId() {
            return client.id;
        }

        static class AlreadySealedException extends Exception {}
    }

    Client checkIn() {
        return new Client(nextClientId++);
    }

    static class InvalidPassportException extends Exception {}

    void verifyPassport(Passport passport) throws InvalidPassportException {
        if (!passports.contains(passport)) {
            throw new InvalidPassportException();
        }
    }
}
