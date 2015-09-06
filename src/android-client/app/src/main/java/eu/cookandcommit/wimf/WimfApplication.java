package eu.cookandcommit.wimf;

import android.app.Application;
import android.widget.ImageView;

public class WimfApplication extends Application {
    public final String AVATAR_DEFAULT_URI = "anonymous";
    public final String NICK_MSG_VAR = "nick";
    public final String AVATAR_URI_MSG_VAR = "avatar_uri";

    Server server = new Server();
    private User user;

    Server getServer() {
        return server;
    }

    public void setUser(User user) {
        this.user = user;
    }
}
