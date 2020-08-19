#ifndef AbstractInterface_H_
#define AbstractInterface_H_


class AbstractInterface {

  public:
  virtual void begin() {};

  virtual void loop()  {};
  virtual void raise() {};
  virtual void reset() {};

};



#endif