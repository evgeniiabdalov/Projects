package Visualization;

import javafx.scene.paint.Color;

public class ColorSetter {

    private Color vertexColor;
    private Color movingAgentColor;
    private Color waitingAgentColor;
    private Color initializedAgentColor;
    private Color arrivedAgentColor;
    private Color markedAgentColor;


    public ColorSetter(){

        this.vertexColor = Color.BLUE;

        this.movingAgentColor = Color.RED;
        this.waitingAgentColor = Color.RED;
        this.initializedAgentColor = Color.RED;
        this.arrivedAgentColor = Color.RED;
        this.markedAgentColor = Color.RED;

    }


    public Color getVertexColor() {
        return vertexColor;
    }

    public Color getMovingAgentColor() {
        return this.movingAgentColor;
    }

    public Color getWaitingAgentColor() {
        return this.waitingAgentColor;
    }

    public Color getInitializedAgentColor() {
        return this.initializedAgentColor;
    }

    public Color getArrivedAgentColor() {
        return arrivedAgentColor;
    }

    public Color getMarkedAgentColor() {
        return markedAgentColor;
    }

    public void setVertexColor(Color vertexColor) {
        this.vertexColor = vertexColor;
    }

    public void setMovingAgentColor(Color movingAgentColor) {
        this.movingAgentColor = movingAgentColor;
    }

    public void setWaitingAgentColor(Color waitingAgentColor) {
        this.waitingAgentColor = waitingAgentColor;
    }

    public void setInitializedAgentColor(Color initializedAgentColor) {
        this.initializedAgentColor = initializedAgentColor;
    }

    public void setArrivedAgentColor(Color arrivedAgentColor) {
        this.arrivedAgentColor = arrivedAgentColor;
    }

    public void setMarkedAgentColor(Color markedAgentColor) {
        this.markedAgentColor = markedAgentColor;
    }


    public boolean equals(ColorSetter anotherColorSetter){

        if(!(this.movingAgentColor.equals(anotherColorSetter.movingAgentColor)))
             return false;

        if(!(this.waitingAgentColor.equals(anotherColorSetter.waitingAgentColor)))
              return false;

        if(!(this.initializedAgentColor.equals(anotherColorSetter.initializedAgentColor)))
            return false;

        if(!(this.arrivedAgentColor.equals(anotherColorSetter.arrivedAgentColor)))
            return false;

        if(!(this.markedAgentColor.equals(anotherColorSetter.markedAgentColor)))
            return false;

        if(!(this.vertexColor.equals(anotherColorSetter.vertexColor)))
            return  false;

        return true;


    }

}
