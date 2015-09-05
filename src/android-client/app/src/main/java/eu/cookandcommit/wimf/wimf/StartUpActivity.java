package eu.cookandcommit.wimf.wimf;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;

public class StartUpActivity extends AppCompatActivity {
    private EditText nickEditText;
    private ImageView avatarImageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_up);

        nickEditText = (EditText) findViewById(R.id.nickEditText);
        assert nickEditText != null;

        avatarImageView = (ImageView) findViewById(R.id.avatarImageView);
        assert avatarImageView != null;
    }

    public void join_joinButton_click(View v) {
        if (TextUtils.isEmpty(nickEditText.getText())) {
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
                }).show();
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

        avatarImageView.setImageURI(avatar);
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

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_start_up, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
