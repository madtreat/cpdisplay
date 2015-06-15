#include <QMainWindow>


class MapController;
class PFDController;

class HDDWindow : public QMainWindow {
   Q_OBJECT

public:
   HDDWindow(QObject* _parent=0);
   ~HDDWindow();

public slots:

private:
   MapController* map;
   PFDController* pfd;
   
};
