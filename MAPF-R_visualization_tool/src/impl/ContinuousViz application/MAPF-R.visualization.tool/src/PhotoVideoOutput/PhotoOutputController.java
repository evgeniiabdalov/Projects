package PhotoVideoOutput;

import javafx.embed.swing.SwingFXUtils;
import javafx.scene.Node;
import javafx.scene.SnapshotParameters;
import javafx.scene.image.WritableImage;

import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;

/**
 * PhotOutputController class is responsible for controlling photo taking process
 * @author Evgenii Abdalov
 */
public class PhotoOutputController {

   private Integer snapCounter;

    /**
    * PhotoOutputController constructor
    */
    public PhotoOutputController(){
       this.snapCounter = 0;
   }

    /**
     * Takes photos of the visualization process on Node
     * @param snapName name of the photo that will be made
     * @param node object that is being photographed
     */
    public void takeSnapshot(String snapName, Node node) throws IOException {

     WritableImage image = node.snapshot(new SnapshotParameters(), null);

     ImageIO.write(SwingFXUtils.fromFXImage(image, null), "png", new File(snapName + " " + this.snapCounter + ".png"));

     this.snapCounter++;

   }


}
