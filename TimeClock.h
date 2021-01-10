//TIMECLOCK
class TimeClock{
  private:
    int h;
    int m;
    int s;
    //char timeString[16];
  public:
    TimeClock(int h, int m, int s){
      this->h=h;
      this->m=m;
      this->s=s;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    int getHour(){
      return h;
    }
    int getMinutes(){
      return m;
    }
    int getSeconds(){
      return s;
    }
    //TODO String Object
//    char* printTime(){
//      static char timeString[16];
//      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
//      lcd.print(timeString);
//    }
    void setHour(int h){
      this->h=h;
    }
    void setMinutes(int m){
      this->m=m;
    }
    void setSeconds(int s){
      this->s=s;
    }
    //TODO REFACTOR checkTime()
    void addHours(int i){
      this->h+=i;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    void addMinutes(int i){
      this->m+=i;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    void addSeconds(int i){
      this->s+=i;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    void minusHours(int i){
      this->h-=i;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    void minusMinutes(int i){
      this->m-=i;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    void minusSeconds(int i){
      this->s-=i;
      //snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", this->h,this->m,this->s);
    }
    void checkTime(){
      if(this->s==60){
        this->m++;
        this->s=0;
      }
      if(this->m==60){
        this->h++;
        this->m=0;
      }
      if(this->h>24){
        this->h=0;
      }
     if(this->s<0){
	      this->m--;
        this->s=60;
      }		
     if(this->m<0){
	      this->h--;
        this->m=60;
      }
     if(this->h<0){
        this->h=24;
      }		
    }
};
