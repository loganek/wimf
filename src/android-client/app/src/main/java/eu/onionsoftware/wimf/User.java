package eu.onionsoftware.wimf;

import com.google.android.gms.maps.model.LatLng;

import java.util.ArrayList;

public class User {
    private int id;
    private LatLng location;
    private ArrayList<Chat> chats;
    private String nick;

    public User(String nick, int id) {
        this.nick = nick;
        this.id = id;
    }

    public Chat getChat(User friend) {
        for (Chat chat : chats) {
            if (chat.getFriend().equals(friend)) {
                return chat;
            }
        }

        return createChat(friend);
    }

    public void removeChat(User friend) {
        chats.remove(friend);
    }

    private Chat createChat(User friend) {
        Chat chat = new Chat(friend);
        chats.add(chat);
        return chat;
    }

    public LatLng getLocation() {
        return location;
    }

    public void updateLocation(LatLng location) {
        this.location = location;
    }

    public String getNick() {
        return nick;
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof User)) {
            return false;
        }

        final User other = (User) obj;
        return other.id == this.id;
    }
}