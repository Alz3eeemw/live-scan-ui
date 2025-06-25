#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <BluetoothSerial.h>
#include "config.h"

/**
 * @brief يدير اتصال البلوتوث وإرسال بيانات المسح.
 */
class CommunicationManager {
public:
    /**
     * @brief يبدأ البلوتوث باسم الجهاز المحدد.
     */
    void begin();

    /**
     * @brief يتحقق من وجود جهاز متصل.
     * @return true إذا كان هناك اتصال، false خلاف ذلك.
     */
    bool isConnected();

    /**
     * @brief يرسل بيانات المسح المباشر بشفرة JSON.
     * @param data هيكل بيانات الحساس المراد إرسالها.
     */
    void sendLiveScanData(const SensorData& data);

    /**
     * @brief يرسل بيانات نقطة مسح ثنائية الأبعاد.
     * @param x إحداثي X.
     * @param y إحداثي Y.
     * @param value قيمة الحساس في هذه النقطة.
     */
    void sendPointData(int x, int y, float value);

    /**
     * @brief يرسل أمر ضبط أبعاد الشبكة لتطبيق 3D Visualizer.
     * @param width عرض الشبكة (عدد الأعمدة).
     * @param height طول الشبكة (عدد الصفوف).
     */
    void sendGridSetupCommand(int width, int height);

private:
    BluetoothSerial SerialBT;  // كائن البلوتوث التسلسلي للتحكم الكامل باتصال ESP32
};

#endif  // COMMUNICATION_MANAGER_H
