package Visualization;

import Business.Vertex;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;

import java.util.ArrayList;
import java.util.List;

public class GraphVisual {

    private List<Line>   edgeLines;
    private List<Circle> vertexCircles;

    private boolean isVisible;

    public GraphVisual()
    {
        isVisible = true;
        edgeLines = new ArrayList<>();
        vertexCircles = new ArrayList<>();
    }

    public void addLine(Vertex fromVertex, Vertex toVertex)
    {

        Double x1 = fromVertex.getXCoordinate() * 100;
        Double y1 = fromVertex.getYCoordinate() * 100;
        Double x2 = toVertex.getXCoordinate() * 100;
        Double y2 = toVertex.getYCoordinate() * 100;

        Line edgeLine = new Line(x1, y1, x2, y2);
        edgeLine.setStrokeWidth(1);

        this.edgeLines.add(edgeLine);

    }

    public void addCircle(Vertex vertex){

        Circle vertexCircle = new Circle();

        vertexCircle.setId("vertex" + vertex.getIndex().toString());
        vertexCircle.setCenterX(vertex.getXCoordinate() * 100);
        vertexCircle.setCenterY(vertex.getYCoordinate() * 100);
        vertexCircle.setRadius(10);
        vertexCircle.setStroke(Color.GREEN);
        vertexCircle.setFill(Color.BLUE);

        this.vertexCircles.add(vertexCircle);
    }

    public void changeVisibility(){

        if(this.isVisible) {

            makeInvisible();
            this.isVisible = false;

        }
        else {

            makeVisible();
            this.isVisible = true;

        }

    }

    private void makeVisible(){

        for( Line edge : this.edgeLines )
            edge.setVisible(true);

        for( Circle vertex : this.vertexCircles )
            vertex.setVisible(true);

    }

    private void makeInvisible(){

        for( Line edge : this.edgeLines )
            edge.setVisible(false);

        for( Circle vertex : this.vertexCircles )
            vertex.setVisible(false);

    }

    public List<Circle> getVertexCircles(){
        return this.vertexCircles;
    }

    public List<Line> getEdgeLines(){
        return this.edgeLines;
    }

    public void changeVertexColor(Color color){

        for(Circle vertexCircle : this.vertexCircles){
            vertexCircle.setFill(color);
        }

    }



}
