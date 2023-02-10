 #include <glib.h>
 #include <gst/gst.h>
 #include <gst/video/videooverlay.h>

 #include <QApplication>
 #include <QTimer>
 #include <QWidget>

 #include <QMainWindow>

 #include "mainwindow.h"

 int main(int argc, char *argv[])
 {
   gst_init (&argc, &argv);
   QApplication app(argc, argv);
   app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit ()));

   // prepare the pipeline

   GstElement *pipeline = gst_pipeline_new ("xvoverlay");
   GstElement *src = gst_element_factory_make ("videotestsrc", NULL);
   GstElement *sink = gst_element_factory_make ("d3d11videosink", NULL);
   gst_bin_add_many (GST_BIN (pipeline), src, sink, NULL);
   gst_element_link (src, sink);

   // prepare the ui

   MainWindow w;
   w.show();
   WId xwinid = w.widget->winId();

   gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), xwinid);

   // run the pipeline

   GstStateChangeReturn sret = gst_element_set_state (pipeline,
       GST_STATE_PLAYING);
   if (sret == GST_STATE_CHANGE_FAILURE) {
     gst_element_set_state (pipeline, GST_STATE_NULL);
     gst_object_unref (pipeline);
     // Exit application
     QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
   }

   int ret = app.exec();

   w.hide();
   gst_element_set_state (pipeline, GST_STATE_NULL);
   gst_object_unref (pipeline);

   return ret;
 }