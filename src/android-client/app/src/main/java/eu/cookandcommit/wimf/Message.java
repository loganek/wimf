package eu.cookandcommit.wimf;

import java.util.Date;

enum MessageType {
    INCOMING,
    OUTCOMING
}

class Message {
    private String text;
    private MessageType type;
    private Date date;

    public Message(String text, MessageType type) {
        this.type = type;
        this.text = text;
        this.date = new Date();
    }

    public String getText() {
        return text;
    }

    public Date getDate() {
        return date;
    }

    public MessageType getType() {
        return type;
    }
}
