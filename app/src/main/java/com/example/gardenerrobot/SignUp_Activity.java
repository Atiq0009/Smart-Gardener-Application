package com.example.gardenerrobot;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

import com.basgeekball.awesomevalidation.AwesomeValidation;
import com.basgeekball.awesomevalidation.ValidationStyle;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.HashMap;
import java.util.Map;

public class SignUp_Activity extends AppCompatActivity{
    private EditText editEmail,editpassword, editUserName, editRetypePass;
    private CheckBox editCheckTerm;
    private Button editSign, editAlready;
    private boolean checked = false;
    AwesomeValidation validCheck;

    private FirebaseAuth mAuth;
    private FirebaseAuth.AuthStateListener firebaseAuthListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up_);

        validCheck = new AwesomeValidation(ValidationStyle.BASIC);

        editEmail = (EditText) findViewById(R.id.signEmailId);
        editpassword = (EditText) findViewById(R.id.signPassword);
        editRetypePass = (EditText) findViewById(R.id.signrytypePassward);
        editUserName = (EditText) findViewById(R.id.signUserName);

        String regexPassword = "([A-Z])\\w+";
        validCheck.addValidation(SignUp_Activity.this,R.id.signUserName, "[a-zA-Z\\s]+", R.string.errorUserName);
        validCheck.addValidation(SignUp_Activity.this, R.id.signEmailId, Patterns.EMAIL_ADDRESS, R.string.errorEmail);
        validCheck.addValidation(SignUp_Activity.this, R.id.signPassword, regexPassword, R.string.errorPassword);
        validCheck.addValidation(SignUp_Activity.this,R.id.signrytypePassward,R.id.signPassword, R.string.errorRetypePass);

        editCheckTerm = (CheckBox) findViewById(R.id.TermsCheck);

        editCheckTerm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (editCheckTerm.isEnabled()){
                    checked = true;
                    Toast.makeText(SignUp_Activity.this,"Checked",Toast.LENGTH_SHORT).show();
                }else{
                    checked = false;
                    Toast.makeText(SignUp_Activity.this,"Not Checked",Toast.LENGTH_SHORT).show();
                }
            }
        });


        editSign = (Button) findViewById(R.id.signUp);
        editAlready = (Button) findViewById(R.id.Already);

        editSign.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openRegister();
            }
        });
        editAlready.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                openLoginBack();
            }
        });

        mAuth = FirebaseAuth.getInstance();
        firebaseAuthListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
                if (user != null){
                    Intent intentUser = new Intent(SignUp_Activity.this, Home_Activity.class);
                    startActivity(intentUser);
                    finish();
                    return;
                }
            }
        };

    }

    @Override
    protected void onStart() {
        super.onStart();
        mAuth.addAuthStateListener(firebaseAuthListener);
    }

    @Override
    protected void onStop() {
        super.onStop();
        mAuth.removeAuthStateListener(firebaseAuthListener);
    }

    public void openRegister(){
        final String username = editUserName.getText().toString().trim();
        String password = editpassword.getText().toString().trim();
        final String email = editEmail.getText().toString().trim();

        if(validCheck.validate()){
            if(checked == true){
                mAuth.createUserWithEmailAndPassword(email,password).addOnCompleteListener(SignUp_Activity.this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (!task.isSuccessful()){
                            Toast.makeText(SignUp_Activity.this,"SignUp Error",Toast.LENGTH_SHORT).show();
                        }else{
                            String user_id = mAuth.getCurrentUser() .getUid();
                            DatabaseReference current_user_id = FirebaseDatabase.getInstance().getReference().child("Users").child(user_id);

                            Map newOwner = new HashMap();
                            newOwner.put("UserName", username);
                            newOwner.put("Email", email);

                            current_user_id.setValue(newOwner);
                            Intent openHome = new Intent(SignUp_Activity.this, MainActivity.class);
                            startActivity(openHome);
                            finish();
                        }
                    }
                });

            }else{
                Toast.makeText(SignUp_Activity.this,"Allow Terms and condition",Toast.LENGTH_SHORT).show();
            }

        }else{
            Toast.makeText(SignUp_Activity.this,"Error",Toast.LENGTH_SHORT).show();
        }
    }

    public void openLoginBack() {
        Intent i = new Intent(this, MainActivity.class);
        startActivity(i);
        finish();
    }
}
