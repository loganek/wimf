package eu.cookandcommit.wimf;

import java.util.ArrayList;

public class User {
    private int id;
    private ArrayList<Chat> chats;

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

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof User)) {
            return false;
        }

        final User other = (User) obj;
        return other.id == this.id;
    }
}
