package com.example.gardenerrobot;

public class timeStore {
    private String DCAfternoon;
    private String DCNoon;
    private String DCMorning;
    private String SecFirst;
    private String SecSecond;

    public timeStore(){

    }

    public String getDCAfternoon() {
        return DCAfternoon;
    }

    public void setDCAfternoon(String DCAfernoon) {
        this.DCAfternoon = DCAfernoon;
    }

    public String getDCNoon() {
        return DCNoon;
    }

    public void setDCNoon(String DCNoon) {
        this.DCNoon = DCNoon;
    }

    public String getDCMorning() {
        return DCMorning;
    }

    public void setDCMorning(String DCMorning) {
        this.DCMorning = DCMorning;
    }

    public String getSecFirst() {
        return SecFirst;
    }

    public void setSecFirst(String secFirst) {
        SecFirst = secFirst;
    }

    public String getSecSecond() {
        return SecSecond;
    }

    public void setSecSecond(String secSecond) {
        SecSecond = secSecond;
    }
}
