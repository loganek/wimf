package eu.onionsoftware.wimf;

import android.app.Application;
import android.widget.ImageView;

public class WimfApplication extends Application {
    public final String AVATAR_DEFAULT_URI = "anonymous";

    Server server = new Server();
    private User user;

    Server getServer() {
        return server;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public User getUser() {
        return user;
    }
}
