#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
class QTimer;

class Window: public QOpenGLWindow {
    //Q_OBJECT
    public:
        Window();
        void resizeGL(int w, int h);
        void paintGL();
    protected:
        void mousePressEvent(QMouseEvent* ev);
        void mouseMoveEvent(QMouseEvent* ev);
        void mouseReleaseEvent(QMouseEvent* ev);
        void wheelEvent(QWheelEvent* ev);
        void keyPressEvent(QKeyEvent* ev);
    private:
        void updateTransform();
        QPointF m_origin = QPointF(0,0);
        qreal m_radius = 500;
        qreal m_zoom = 1.0;
        QTransform m_transform;

        bool m_mouseMoving = false;
        QPointF m_mouse_pos;
        float m_t = 0;
        float m_dt = 0.01;


        QImage m_image_values;
        QImage m_image_dfdt;

        QTimer* m_timer;
        void initialize();
        bool m_autostep = false;
        bool m_recording= false;
        bool m_current_frame_recorded = true;

        void step();

        public slots:
            void toggleRecording();
            void startRecording();
            void stopRecording();
};


#endif
