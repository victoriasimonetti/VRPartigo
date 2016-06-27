class Client {
private:
	int id;
	double initialTime;
	double endTime;
	double durationTime;

public:
	Client(int id, double initialTime, double endTime, double durationTime);
	int getId();
	double getInitialTime();
	double getEndTime();
	double getDurationTime(); 

};
