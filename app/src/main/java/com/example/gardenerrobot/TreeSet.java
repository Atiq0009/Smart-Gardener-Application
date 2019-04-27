package com.example.gardenerrobot;

public class TreeSet {
    private String plantName;
    private String plantDate;
    private String plantRf;

    public TreeSet(String plantName, String plantDate, String plantRf) {
        this.plantName = plantName;
        this.plantDate = plantDate;
        this.plantRf = plantRf;
    }

    public String getPlantName() {
        return plantName;
    }

    public void setPlantName(String plantName) {
        this.plantName = plantName;
    }

    public String getPlantDate() {
        return plantDate;
    }

    public void setPlantDate(String plantDate) {
        this.plantDate = plantDate;
    }

    public String getPlantRf() {
        return plantRf;
    }

    public void setPlantRf(String plantRf) {
        this.plantRf = plantRf;
    }
}
