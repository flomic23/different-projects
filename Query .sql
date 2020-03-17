USE MySession;
SELECT  DiscName FROM Disciplines;
SELECT  IdDisc FROM Balls;
SELECT NumSt FROM Balls WHERE IdDisc=1;
SELECT NumDir, COUNT(NumGr) FROM Groups GROUP BY NumDir;
--1--
SELECT Title FROM Directions;
SELECT NumGr FROM Groups;
SELECT FIO FROM Students;
SELECT NumSt FROM Balls;
SELECT NumDir FROM Uplans;--without DISTINCT 
SELECT DISTINCT NumDir FROM Uplans;--with DISTINCT 
SELECT DISTINCT Semestr FROM Uplans;--3.6
SELECT FIO FROM Students WHERE NumGr='13504/1';--3.7
SELECT DISTINCT NumDisc FROM Uplans WHERE NumDir=230100 AND Semestr=1;--3.8
SELECT NumGr, COUNT(NumSt) FROM Students GROUP BY NumGr;--3.9
--3.10--
SELECT NumGr, COUNT(Students.NumSt) 
	FROM Students,Balls 
	WHERE Balls.NumSt=Students.NumSt
    GROUP BY NumGr 
	HAVING COUNT(Students.NumSt)>0;
--3.11--
SELECT NumGr, COUNT(Students.NumSt) 
	FROM Students,Balls 
	WHERE Balls.NumSt=Students.NumSt
    GROUP BY NumGr 
	HAVING COUNT(Students.NumSt)>1;
--4.1--
SELECT DISTINCT FIO
	FROM Students,Balls 
	WHERE  Balls.NumSt=Students.NumSt;
--4.2--
SELECT DISTINCT DiscName
	FROM Disciplines,Balls 
	WHERE  Balls.IdDisc=Disciplines.NumDisc;
--4.3--
SELECT DISTINCT DiscName
	FROM Disciplines,Uplans 
	WHERE  Disciplines.NumDisc=Uplans.NumDisc AND Uplans.NumDir=230100;
--4.4--
SELECT  FIO
	FROM Students,Balls 
	WHERE  Students.NumSt=Balls.NumSt 
	--AND
	GROUP BY FIO
	HAVING COUNT(Balls.IdBall)>1;
--4.5--
SELECT DISTINCT FIO
	FROM Students,Balls 
	WHERE  Students.NumSt=Balls.NumSt 
	AND Balls.Ball= (Select MIN(Ball) FROM Balls);
--4.6--
SELECT DISTINCT FIO
	FROM Students,Balls 
	WHERE  Students.NumSt=Balls.NumSt 
	AND Balls.Ball= (Select MAX(Ball) FROM Balls);
--4.7--
SELECT DISTINCT( Students.NumGr)
	FROM Students,Balls,Disciplines,Uplans
	WHERE Uplans.Semestr=1 --
	AND Uplans.NumDisc=Disciplines.NumDisc--
	AND Disciplines.DiscName = 'Физика'--
	AND Disciplines.NumDisc=Balls.IdDisc--
	AND Balls.NumSt=Students.NumSt--
	GROUP BY Students.NumGr
	HAVING COUNT(Balls.NumSt)>1;
--4.8--
SELECT DISTINCT FIO
	FROM Students,Balls 
	WHERE  Students.NumSt=Balls.NumSt 
	GROUP BY FIO
	HAVING SUM(Balls.Ball) > 9;
--4.9--
SELECT DISTINCT Semestr
	FROM Uplans,Balls
	WHERE Uplans.IdDisc=Balls.IdDisc
	GROUP BY Semestr
	HAVING COUNT(Balls.NumSt) > 1;
