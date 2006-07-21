CREATE DATABASE IF NOT EXISTS strongbox_demo;
USE strongbox_demo;

CREATE TABLE IF NOT EXISTS shared (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(255) NOT NULL,
    descripcion TEXT,
    nReservado INT,
    szReservado TEXT,
    INDEX (nReservado)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS usuario (
    documento BIGINT NOT NULL PRIMARY KEY,
    nombre VARCHAR(255),
    apellido VARCHAR(255),
    grupo BIGINT,
    observaciones TEXT
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS sucesos (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    usuario BIGINT NOT NULL,
    fecha DATETIME,
    dinero DOUBLE,
    salida DATETIME,
    responsable BIGINT UNSIGNED,
    INDEX (usuario)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS huella (
    documento BIGINT NOT NULL,
    dedo INT NOT NULL,
    ancho INT,
    alto INT,
    resolucion INT,
    huella LONGTEXT,
    PRIMARY KEY (documento, dedo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS template (
    documento BIGINT NOT NULL,
    dedo INT NOT NULL,
    huella LONGTEXT,
    PRIMARY KEY (documento, dedo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS propiedades (
    codigo BIGINT NOT NULL PRIMARY KEY,
    usuario BIGINT,
    fecha DATETIME
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS busqueda (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    huella LONGTEXT,
    usuario BIGINT,
    suceso BIGINT,
    fecha DATETIME
) ENGINE=MyISAM;

INSERT IGNORE INTO shared (id, nombre, descripcion, nReservado, szReservado)
VALUES (9001, 'Demo group', 'Synthetic access records', 0, '');

-- Application profile for the fictitious, separately provisioned database account.
INSERT IGNORE INTO shared (id, nombre, descripcion, nReservado, szReservado)
VALUES (9002, 'strongbox_demo', '0', 2, '0');

INSERT IGNORE INTO usuario (documento, nombre, apellido, grupo, observaciones)
VALUES (-9001, 'Demo', 'User', 9001, 'Fictional record');

INSERT IGNORE INTO sucesos (id, usuario, fecha, dinero, salida)
VALUES (9001, -9001, '2006-07-21 08:00:00', 0, '2006-07-21 09:00:00');
