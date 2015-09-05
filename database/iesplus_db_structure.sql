# DEFINING TABLES
# centros
CREATE TABLE centros (
	id 	CHAR(6) PRIMARY KEY,
	tipo 	CHAR(4) NOT NULL,
       	nombre 	TEXT NOT NULL,
	abr 	CHAR(6) NOT NULL
);

# acad
CREATE TABLE acad (
	acad 	INTEGER PRIMARY KEY,
	nombre 	CHAR(7) NOT NULL,
       	ini 	CHAR(7),
	inilect CHAR(7),
	finlect CHAR(7),
	fin 	CHAR(7)
);

# estudios
CREATE TABLE estudios (
	estudio CHAR(3) PRIMARY KEY,
	orden 	INTEGER NOT NULL,
       	nombre 	TEXT NOT NULL,
	abr 	CHAR(5) NOT NULL,
	abrmin 	CHAR(3) NOT NULL,
	simb 	CHAR NOT NULL
);

# xcurso
CREATE TABLE xcurso (
	id 	CHAR PRIMARY KEY,
	orden	INTEGER,
	nombre 	CHAR(7),
	abr 	CHAR(2),
	simb 	CHAR
);

# estcur
CREATE TABLE estcur (
	estudio	CHAR(3) REFERENCES estudios,
	curso 	CHAR REFERENCES xcurso,
	cod 	CHAR(2) NOT NULL,
	PRIMARY KEY (estudio, curso)
);

# departamentos
CREATE TABLE deptos (
	depto	CHAR(2) PRIMARY KEY,
	nombre 	TEXT NOT NULL,
	cod 	CHAR(2) NOT NULL
);

# materias
CREATE TABLE materias (
	materia	CHAR(5) PRIMARY KEY,
	nombre 	TEXT NOT NULL,
	cod 	CHAR(5) NOT NULL
);


#----------------------------------------------------------



# personas
CREATE TABLE personas (
	persona INTEGER PRIMARY KEY,
       	nom TEXT NOT NULL,
	ape1 TEXT NOT NULL,
	ape2 TEXT,
	sexo CHAR NOT NULL
);

# profesores
CREATE TABLE profesores (
	prof	INTEGER NOT NULL PRIMARY KEY,
       	persona INTEGER NOT NULL REFERENCES personas
);

# alumnos
CREATE TABLE alumnos (
	alumno 	INTEGER PRIMARY KEY,
       	persona INTEGER NOT NULL REFERENCES personas,
	numexp 	INTEGER
);

# acadcentro
CREATE TABLE acadcentro (
	acad 	INTEGER REFERENCES acad,
	centro 	CHAR(5) REFERENCES centros,
	PRIMARY KEY (acad,centro)
);

# acadestudio
CREATE TABLE acadestudio (
	acad 	INTEGER,
	centro 	CHAR(5),
	estudio	CHAR(3) REFERENCES estudios,
	PRIMARY KEY (acad,centro,estudio),
	FOREIGN KEY (acad,centro) REFERENCES acadcentro (acad,centro)
);

# acadcurso
CREATE TABLE acadcurso (
	acad 	INTEGER,
	centro 	CHAR(5),
	estudio	CHAR(3),
	curso 	CHAR REFERENCES xcurso,
	PRIMARY KEY (acad,centro,estudio),
	FOREIGN KEY (estudio,curso) REFERENCES estcur (estudio,curso),
	FOREIGN KEY (acad,centro,estudio) REFERENCES acadestudio (acad,centro,estudio)
);

# acaddepto
CREATE TABLE acaddepto (
	acad 	INTEGER,
	centro 	CHAR(5),
	depto	CHAR(3),
	PRIMARY KEY (acad,centro,depto),
	FOREIGN KEY (acad,centro) REFERENCES acadcentro (acad,centro)
);

# acadmateria
CREATE TABLE acadmateria (
	acad 	INTEGER,
	centro 	CHAR(5),
	estudio	CHAR(3),
	curso	CHAR,
	materia	CHAR(5) REFERENCES materias,
	PRIMARY KEY (acad,centro,estudio,curso,materia),
	FOREIGN KEY (acad,centro,estudio,curso) REFERENCES acadcurso (acad,centro,estudio,curso)
);


#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
# POPULATE TABLES
# centros
INSERT INTO centros VALUES('IESMB', 'IES', 'Mariano Baquero', 'IESMB');
# acad
INSERT INTO acad VALUES(2012, '2012-13', '', '', '', '');
# estudios
INSERT INTO estudios VALUES('ESO', 1, 'Educación Secundaria', 'ESO', 'ESO', 'E');
INSERT INTO estudios VALUES('BAC', 2, 'Bachillerato', 'Bach.', 'BAC', 'B');
# cursos
INSERT INTO xcurso VALUES('1', 1, 'Primero', '1º', '1');
INSERT INTO xcurso VALUES('2', 2, 'Segundo', '2º', '2');
INSERT INTO xcurso VALUES('3', 3, 'Tercero', '3º', '3');
INSERT INTO xcurso VALUES('4', 4, 'Cuarto', '4º', '4');
# estcur
INSERT INTO estcur VALUES('ESO', '1', 'E1');
INSERT INTO estcur VALUES('ESO', '2', 'E2');
INSERT INTO estcur VALUES('ESO', '3', 'E3');
INSERT INTO estcur VALUES('ESO', '4', 'E4');
INSERT INTO estcur VALUES('BAC', '1', 'B1');
INSERT INTO estcur VALUES('BAC', '2', 'B2');
# deptos
INSERT INTO deptos VALUES('FQ', 'Física y Química', 'FQ');
INSERT INTO deptos VALUES('BG', 'Biología y Geología', 'BG');
INSERT INTO deptos VALUES('GH', 'Geografía e Historia', 'GH');
INSERT INTO deptos VALUES('IN', 'Inglés', 'IN');
INSERT INTO deptos VALUES('FR', 'Francés', 'FR');
INSERT INTO deptos VALUES('MU', 'Música', 'MU');
INSERT INTO deptos VALUES('EF', 'Educación Física', 'EF');
INSERT INTO deptos VALUES('TE', 'Tecnología', 'TE');
INSERT INTO deptos VALUES('LA', 'Latín', 'LA');
INSERT INTO deptos VALUES('GR', 'Griego', 'GR');
INSERT INTO deptos VALUES('OR', 'Orientación', 'OR');
INSERT INTO deptos VALUES('RE', 'Religión', 'RE');
INSERT INTO deptos VALUES('DI', 'Dibujo', 'DI');

# materias
INSERT INTO materias VALUES('E3FQ', 'Física y Química, 3º ESO', 'E3FQ');
INSERT INTO materias VALUES('E4FQ', 'Física y Química, 4º ESO', 'E4FQ');
INSERT INTO materias VALUES('E4AFQ', 'Ampliación y Profundización de Física y Química, 4º ESO', 'E4APFQ');
INSERT INTO materias VALUES('B1FQ', 'Física y Química, 1º Bach.', 'B1FQ');
INSERT INTO materias VALUES('B2FIS', 'Física, 2º Bach.', 'B2FIS');
INSERT INTO materias VALUES('B2QUI', 'Química, 2º Bach.', 'B2QUI');

########################################################################
## THE LAST ONE
# system:
CREATE TABLE system (
	name CHAR(7),
       	ver_lev1 INTEGER,
       	ver_lev2 INTEGER,
       	rev INTEGER
);

# POPULATE system
INSERT INTO system VALUES('IesPlus', 0, 0, 0);

