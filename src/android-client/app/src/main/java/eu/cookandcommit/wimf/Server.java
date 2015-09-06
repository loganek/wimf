package eu.cookandcommit.wimf;

import com.google.android.gms.maps.model.LatLng;

import java.util.ArrayList;
import java.util.List;

interface ProtocolListener {
    void userLocationChanged(User user, LatLng location);
    void userRegistered(int id, String nick);
}

public class Server {
    List<ProtocolListener> listeners = new ArrayList<ProtocolListener>();

    public void addListener(ProtocolListener listener) {
        listeners.add(listener);
    }

    public void requestClients(LatLng location) {
    }

    public void connect() {
    }

    public void registerRequest(String nick) {
    }
}
