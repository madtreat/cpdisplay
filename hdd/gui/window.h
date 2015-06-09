#include <QWindow>


class PFDWidget;
class MapWidget;

class HDDWindow : public QWindow {
   Q_OBJECT

public:
   HDDWindow();
   ~HDDWindow();

public slots:

private:
   PFDWidget* pfdWidget;
};
