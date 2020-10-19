package PhotoVideoOutput;

import javafx.embed.swing.SwingFXUtils;
import javafx.scene.Node;
import javafx.scene.SnapshotParameters;
import javafx.scene.image.WritableImage;

import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;


public class PhotoOutputController {

   private Integer snapCounter;

   public PhotoOutputController(){
       this.snapCounter = 0;
   }

   public void takeSnapshot(String snapName, Node node) throws IOException {

     WritableImage image = node.snapshot(new SnapshotParameters(), null);

     ImageIO.write(SwingFXUtils.fromFXImage(image, null), "png", new File(snapName + " " + this.snapCounter + ".png"));

     this.snapCounter++;

   }


}
