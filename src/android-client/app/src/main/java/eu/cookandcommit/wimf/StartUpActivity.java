package eu.cookandcommit.wimf;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;

import com.google.android.gms.maps.model.LatLng;

public class StartUpActivity extends Activity implements ProtocolListener {
    private EditText nickEditText;
    private ImageView avatarImageView;
    WimfApplication application;
    String avatarImage;
    private Server server;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_up);

        nickEditText = (EditText) findViewById(R.id.nickEditText);
        assert nickEditText != null;

        avatarImageView = (ImageView) findViewById(R.id.avatarImageView);
        assert avatarImageView != null;

        application = (WimfApplication) getApplication();
        assert application != null;

        server = application.getServer();
        server.addListener(this);

        avatarImage = application.AVATAR_DEFAULT_URI;

        if (BuildConfig.DEBUG) {
            Intent main = new Intent(this, ChatActivity.class);
            startActivity(main);
        }
    }

    public void join_joinButton_click(View v) {
        String nickName = nickEditText.getText().toString();
        if (nickIsValid(nickName)) {
            server.registerRequest(nickName);
        } else {
            showEmptyNickWarning();
        }
    }

    public void pickAvatar_avatarImageView_click(View v) {
        new AlertDialog.Builder(StartUpActivity.this)
                .setTitle("Select avatar source")
                .setItems(R.array.avatar_source_array, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Intent pickAvatar = null;
                        switch (which) {
                            case 0:  // camera
                                pickAvatar = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                                break;
                            case 1:  // gallery
                                pickAvatar = new Intent(Intent.ACTION_PICK,
                                        MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                                break;
                        }
                        if (pickAvatar != null) {
                            startActivityForResult(pickAvatar, which);
                        }
                    }
                }).show()
                .setCanceledOnTouchOutside(true);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent imageReturnedIntent) {
        super.onActivityResult(requestCode, resultCode, imageReturnedIntent);
        Uri avatar = null;
        switch (requestCode) {
            case 0:
                if (resultCode == RESULT_OK) {
                    avatar = imageReturnedIntent.getData();
                }

                break;
            case 1:
                if (resultCode == RESULT_OK) {
                    avatar = imageReturnedIntent.getData();
                }
                break;
        }

        if (avatar != null) {
            avatarImageView.setImageURI(avatar);
            avatarImage = avatar.toString();
        }
    }

    private void showEmptyNickWarning() {
        new AlertDialog.Builder(StartUpActivity.this)
                .setTitle("Invalid nick")
                .setMessage("Nick can not be empty")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        // do nothing, just close this dialog
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_info)
                .show()
                .setCanceledOnTouchOutside(true);
    }

    private boolean nickIsValid(String nick) {
        return !TextUtils.isEmpty(nick);
    }

    @Override
    public void userLocationChanged(User user, LatLng location) {
    }

    @Override
    public void userRegistered(int id, String nickName) {
        application.setUser(new User(nickName, id));
        Intent main = new Intent(this, MainActivity.class);
        startActivity(main);
    }
}
