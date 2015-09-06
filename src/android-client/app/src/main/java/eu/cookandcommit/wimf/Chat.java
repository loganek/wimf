package eu.cookandcommit.wimf;

import java.util.ArrayList;

public class Chat {
    private User friend;
    private ArrayList<Message> messages;

    public Chat(User friend) {
        this.friend = friend;
    }

    public void addMessage(String text, MessageType type) {
        messages.add(new Message(text, type));
    }


    public User getFriend() {
        return friend;
    }
}
