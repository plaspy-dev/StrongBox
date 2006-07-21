#pragma once
# include "../errorreport/functionstacktrace.h"

// messages
# define	WM_INSERTODS		WM_USER+9001
# define	WM_NEWXML			WM_USER+9002
# define	WM_VIEWNOTIFICACION WM_USER+9003
# define	WM_QUERYCONECTOR	WM_USER+9004
# define	WM_SETSTATUS		WM_USER+9005
# define	WM_QUERYUSUARIO		WM_USER+9006
# define	WM_STATUSBAR		WM_USER+9007
# define	WM_PAGINAWEB		WM_USER+9008
# define	WM_RESULTADOS		WM_USER+9009
# define	WM_TREADRESULT		WM_USER+9010
# define	WM_CLOSEEX			WM_USER+9011
# define	WM_QUERYREPORT		WM_USER+9012
# define	WM_QUERYREPORTEX	WM_USER+9013
# define	WM_QUERYREPORTEXTRA	WM_USER+9014
# define	WM_FILECHANGED		WM_USER+9015
# define	WM_IMPORTFILE		WM_USER+9016
# define	WM_QUERYALMACEN		WM_USER+9017
# define	WM_UPDATEALMACEN	WM_USER+9018
# define	WM_INACTIVIDAD		WM_USER+9019
# define	WM_QUERYSS			WM_USER+9020
# define	WM_UPDATESS			WM_USER+9021
# define	WM_REGISTRARUSUARIO WM_USER+9022
# define	WM_QUERYCIERRE		WM_USER+9023
# define	WM_QUERYTIME		WM_USER+9024
# define	WM_QUERYTABLAS		WM_USER+9025
// inicial update

# define	ID_CONFIGURACION		0
# define	ID_REPORTE				1
# define	ID_COMBO_MODELOS		2
# define	ID_MARCAS				3
# define	ID_CIUDADES				4
# define	ID_CAD					5
# define	ID_RECIBIDO				6
# define	ID_TECNICOS				7
# define	ID_ESTADOFINAL			8
# define	ID_PROBLEMAS			9
# define	ID_GARANTIA				10
# define	ID_INSPECCIONFISICA		12
# define	ID_CODIGOSREPARACION	13
# define	ID_SINTOMAS				14
# define	ID_PARTES				15
# define	ID_FESTIVOS				16
# define	ID_SSTIPO				17
# define	ID_ITEMSTIPO			18
# define	ID_RESALMACEN			19
# define	ID_COMPATIBLES			20
// aumentar por que este es el delimitador del array
# define	ID_ESTADOS				21

// grupos

# define	GRUPO_ADMINISTRADOR		1
# define	GRUPO_DIGITADOR			2
# define	GRUPO_BODEGA			4
# define	GRUPO_CLAIMS			8
# define	GRUPO_INFORMADORES		16
# define	GRUPO_CORRECTORES		32
# define	GRUPO_ADMINBODEGA		64


// ya no se usan pero bueno se dejan por respetar la version anterior
// busqueda

# define	ID_DOCUMENTO		100
# define	ID_FIND_ODS			101
# define	ID_FIND_TELEFONO	102
# define	ID_FIND_FECHAS		103
# define	ID_FIND_EMPLEADOS	104
# define	ID_FIND_TERCEROS	105
# define	ID_FIND_DESCRIPCION 106
# define	ID_FIND_GUIA		107
# define	ID_FIND_PARTES		108
# define	ID_FIND_FACTURA		109

// inserciones

# define	ID_OK					1001
# define	ID_INSERT_USUARIO		1002
# define	ID_INSERT_ODS			1003
# define	ID_INSERT_TELEFONO		1004
# define	ID_INSERT_FECHAS		1005
# define	ID_INSERT_EMPLEADOS		1006
# define	ID_INSERT_TERCEROS		1007
# define	ID_INSERT_DESCRIPCION	1008
# define	ID_INSERT_GUIA			1009
# define	ID_INSERT_PARTES		1010
# define	ID_INSERT_FACTURA		1011


#ifdef _EXPORTCOMPONENTES
#define CLASS_COMPONENTES  __declspec(dllexport)
# pragma message("exportando")
#else
   #define CLASS_COMPONENTES  __declspec(dllimport)
	# pragma message("importando")
#endif