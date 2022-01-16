#pragma once

#include <QObject>

#include <algorithm>

#include <clap/clap.h>

class GuiClient;
class ParameterProxy : public QObject {
   using super = QObject;

   Q_OBJECT
   Q_PROPERTY(uint32_t id READ getId)
   Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
   Q_PROPERTY(QString module READ getModule NOTIFY moduleChanged)
   Q_PROPERTY(double value READ getValue WRITE setValueFromUI NOTIFY valueChanged)
   Q_PROPERTY(double normalizedValue READ getNormalizedValue WRITE setNormalizedValueFromUI NOTIFY
                 valueChanged)
   Q_PROPERTY(double modulation READ getModulation NOTIFY modulationChanged)
   Q_PROPERTY(double normalizedModulation READ getNormalizedModulation NOTIFY modulationChanged)
   Q_PROPERTY(double finalValue READ getFinalValue NOTIFY finalValueChanged)
   Q_PROPERTY(double normalizedFinalValue READ getNormalizedFinalValue NOTIFY finalValueChanged)
   Q_PROPERTY(double minValue READ getMinValue NOTIFY minValueChanged)
   Q_PROPERTY(double maxValue READ getMaxValue NOTIFY maxValueChanged)
   Q_PROPERTY(double defaultValue READ getDefaultValue NOTIFY defaultValueChanged)
   Q_PROPERTY(bool isAdjusting READ isAdjusting WRITE setIsAdjusting NOTIFY isAdjustingChanged)
   Q_PROPERTY(bool isHovered READ isHovered WRITE setIsHovered NOTIFY isHoveredChanged)

public:
   explicit ParameterProxy(GuiClient &client, const clap_param_info &info);
   explicit ParameterProxy(GuiClient &client, clap_id param_id);

   void redefine(const clap_param_info &info);

   uint32_t getId() const { return _id; }
   const QString &getModule() const { return _module; }
   const QString &getName() const { return _name; }

   double getValue() const { return _value; }
   void setValueFromUI(double value);
   void setValueFromPlugin(double value);

   double getNormalizedValue() const { return normalize(getValue()); }
   void setNormalizedValueFromUI(double value) { setValueFromUI(denormalize(value)); }

   double getModulation() const { return _modulation; }
   void setModulationFromPlugin(double mod);

   double getNormalizedModulation() const { return normalize(getModulation()); }

   double getFinalValue() const { return clip(_value + _modulation); }
   double getNormalizedFinalValue() const { return normalize(getFinalValue()); }

   bool isAdjusting() const { return _isAdjusting; }
   void setIsAdjusting(bool isAdjusting);

   double getMinValue() const { return _minValue; }
   void setMinValueFromPlugin(double minValue);
   double getMaxValue() const { return _maxValue; }
   void setMaxValueFromPlugin(double maxValue);
   double getDefaultValue() const { return _defaultValue; }
   void setDefaultValueFromPlugin(double defaultValue);

   double clip(double v) const {
      return std::min<double>(_maxValue, std::max<double>(_minValue, v));
   }
   static double clipNormalized(double v) { return std::min<double>(1., std::max<double>(0., v)); }

   double normalize(double value) const {
      double delta = _maxValue - _minValue;
      return delta != 0 ? ((value - _minValue) / delta) : 0;
   }

   double denormalize(double value) const {
      double delta = _maxValue - _minValue;
      return _minValue + value * delta;
   }

   Q_INVOKABLE void setToDefault();

   bool isHovered() const { return _isHovered; }
   void setIsHovered(bool value);

signals:
   void nameChanged();
   void moduleChanged();
   void valueChanged();
   void modulationChanged();
   void finalValueChanged();
   void minValueChanged();
   void maxValueChanged();
   void defaultValueChanged();
   void isHoveredChanged();
   void isAdjustingChanged();

private:
   GuiClient &_client;
   const uint32_t _id;
   QString _name;
   QString _module;
   double _value = 0;
   double _modulation = 0;
   double _minValue = 0;
   double _maxValue = 0;
   double _defaultValue = 0;
   bool _isAdjusting = false;
   bool _isHovered = false;
};