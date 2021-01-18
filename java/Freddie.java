import java.util.HashSet;
import java.util.Set;

class Freddie {
    private Set<Handle> handles = new HashSet<>();

    static class Handle {
        private Carol.Passport passport;
        private Object channel;

        Handle(Carol.Passport passport, Object channel) {
            this.passport = passport;
            this.channel = channel;
        }

        Carol.Passport getPassport() {
            return passport;
        }

        Object getChannel() {
            return channel;
        }
    }

    Handle makeHandle(Carol.Passport passport, Object channel) throws Carol.Passport.AlreadySealedException {
        passport.seal();
        Handle handle = new Handle(passport, channel);
        handles.add(handle);
        return handle;
    }

    static class InvalidHandleException extends Exception {}

    void verifyHandle(Handle handle) throws InvalidHandleException {
        if (!handles.contains(handle)) {
            throw new InvalidHandleException();
        }
    }
}
