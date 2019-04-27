package com.example.gardenerrobot;

import android.content.Intent;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
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
import java.util.regex.Pattern;

public class Plant_Info_Input extends AppCompatActivity {

    private EditText mPlantName, mPlantDate, mRFNumber;

    private Button addPlant;
    private Button BackPlant;
    AwesomeValidation validCheck;

    private FirebaseAuth mAuth;
    private FirebaseAuth.AuthStateListener firebaseAuthListener;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_plant__info__input);

        validCheck = new AwesomeValidation(ValidationStyle.BASIC);
        mAuth = FirebaseAuth.getInstance();
        firebaseAuthListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
                if (user != null){

                }else{
                    Intent intentUser = new Intent(Plant_Info_Input.this, MainActivity.class);
                    startActivity(intentUser);
                    finish();
                    return;
                }
            }
        };
        mPlantName = (EditText) findViewById(R.id.inpPlantName);
        mPlantDate = (EditText) findViewById(R.id.inpPlantDate);
        mRFNumber = (EditText) findViewById(R.id.inpPlantRFNumber);

        String date = "^\\d{1,2}\\/\\d{1,2}\\/\\d{4}$";
        validCheck.addValidation(Plant_Info_Input.this,R.id.inpPlantName, "[a-zA-Z0-9\\s]+", R.string.errorPlantName);
        validCheck.addValidation(Plant_Info_Input.this, R.id.inpPlantDate, date, R.string.errorDate);
        validCheck.addValidation(Plant_Info_Input.this, R.id.inpPlantRFNumber, "[0-9\\s]+", R.string.errorRFIDNUMBER);

        addPlant = (Button) findViewById(R.id.addNewPlantTime);
        addPlant.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                newPlant();
            }
        });
        BackPlant = (Button) findViewById(R.id.backPlant);
        BackPlant.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                backfunc();
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        mAuth.removeAuthStateListener(firebaseAuthListener);
    }

    @Override
    protected void onStart() {
        super.onStart();
        mAuth.addAuthStateListener(firebaseAuthListener);

    }

    public void newPlant(){
        String plantName = mPlantName.getText().toString().trim();
        String plantDate = mPlantDate.getText().toString().trim();
        String plantRF = mRFNumber.getText().toString().trim();

        if(validCheck.validate()){
            DatabaseReference current_user_id = FirebaseDatabase.getInstance().getReference().child("Tree").child(plantRF);
            TreeSet tree = new TreeSet(plantName,plantDate, plantRF);


            current_user_id.setValue(tree);
            Intent openHome = new Intent(Plant_Info_Input.this, Information.class);
            startActivity(openHome);
            finish();

        }else{
            Toast.makeText(Plant_Info_Input.this,"Error",Toast.LENGTH_SHORT).show();

        }
    }
    public void backfunc(){
        Intent intent1 =new Intent(this, Information.class);
        startActivity(intent1);
        finish();
    }

}
