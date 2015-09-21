package eu.onionsoftware.wimf;

import com.google.android.gms.maps.model.LatLng;

import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

interface ProtocolListener {
    void userLocationChanged(User user, LatLng location);
    void userRegistered(int id, String nick);
}

public class Server {
    List<ProtocolListener> listeners = new ArrayList<ProtocolListener>();
    Socket client = null;
    Thread readThread;
    static final int headerSize = 4;

    public void addListener(ProtocolListener listener) {
        listeners.add(listener);
    }

    public void requestClients(LatLng location) {
    }

    public boolean connect(String host, int port) {
        assert host != null;
        try {
            client = new Socket(host, port);
            readThread = new ReaderThread(client, headerSize, listeners);
            readThread.start();
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public boolean isConnected() {
        return client != null && client.isConnected();
    }

    public boolean registerRequest(String nick) {
        Wimf.WimfInfo.Builder info = Wimf.WimfInfo.newBuilder();
        Wimf.Login.Builder login = Wimf.Login.newBuilder();
        login.setNickname(nick);
        info.setLogin(login.build());

        return sendFrame(info.build());
    }

    public boolean sendFrame(Wimf.WimfInfo frame) {
        byte [] header = new byte[4];

        int size = frame.getSerializedSize();
        for (int i = 0; i < headerSize; i++) {
            header[i] = (byte)(size & 255);
            size >>= 8;
        }

        try {
            client.getOutputStream().write(header);
            frame.writeTo(client.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public void disconnect() {
        if (isConnected()) {
            try {
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

class ReaderThread extends Thread {
    private Socket client;
    private int headerSize;
    List<ProtocolListener> listeners;

    public ReaderThread(Socket client, int headerSize, List<ProtocolListener> listeners) {
        this.client = client;
        this.headerSize = headerSize;
        this.listeners = listeners;
    }

    @Override
    public void run() {
        byte [] headerBuffer = new byte[4];
        while (true) {
            try {
                readRequestedSize(headerBuffer, headerSize);
                int frameSize = 0;
                for (int i = 0; i < headerSize; i++) {
                    frameSize |= headerBuffer[i] << (i << 3);
                }
                // todo verify frameSize < maxSize
                byte [] buffer = new byte[frameSize];
                readRequestedSize(buffer, frameSize);
                processFrame(Wimf.WimfInfo.parseFrom(buffer));
            } catch (IOException e) {
                e.printStackTrace();
                break;
            }
        }
    }

    private void processFrame(Wimf.WimfInfo frame) {
        if (frame.hasLogin()) {
            for (ProtocolListener listener : listeners) {
                listener.userRegistered(frame.getLogin().getId(), frame.getLogin().getNickname());
            }
        }
    }

    private void readRequestedSize(byte[] buffer, int size) throws IOException {
        int cnt = 0;
        while (cnt < size) {
            cnt += client.getInputStream().read(buffer, cnt, size - cnt);
        }
    }
}
