package eu.cookandcommit.wimf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Chat {
    private User friend;
    private List<Message> messages = new ArrayList<Message>();

    public Chat(User friend) {
        this.friend = friend;
    }

    public void addMessage(String text, MessageType type) {
        messages.add(new Message(text, type));
    }

    public User getFriend() {
        return friend;
    }

    public List<Message> getMessages() {
        return Collections.unmodifiableList(messages);
    }
}
