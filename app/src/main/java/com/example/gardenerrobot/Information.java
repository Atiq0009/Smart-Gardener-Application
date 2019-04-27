package com.example.gardenerrobot;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.CardView;
import android.view.View;

public class Information extends AppCompatActivity {
    private CardView tree1;
    private CardView tree2;
    private CardView tree3;
    private CardView tree4;
    private CardView InputInformation;

    private CardView backHome1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_information);
        InputInformation = findViewById(R.id.plantInformation);
        InputInformation.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPlantInformationInput();
            }
        });
        backHome1 = findViewById(R.id.backParent);
        backHome1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                backHome();
            }
        });
        tree1 = findViewById(R.id.treeID1);
        tree1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPlantInformation();
            }
        });

    }

    public void openPlantInformation() {
        Intent intent1 = new Intent(this, Plant_Info.class);
        startActivity(intent1);
    }

    public void openPlantInformationInput() {
        Intent intent2 = new Intent(this, Plant_Info_Input.class);
        startActivity(intent2);
    }

    public void backHome() {
        Intent intent3 = new Intent(this, Home_Activity.class);
        startActivity(intent3);
        finish();
    }
}
