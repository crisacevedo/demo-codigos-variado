
CREATE OR REPLACE VIEW V_solicitudes_alumno
AS
  SELECT alumno.alu_rut rut,
    alu_nombres nombre,
    alu_paterno paterno,
    alu_materno materno ,
    car_nom nom_carrera,
    plan_estudio.plan_id plan_de_estudio,
    COUNT(alu_sol.als_rut) numerosolicitudes
  FROM alu_sol,
    carrera,
    alumno,
    plan_estudio
  WHERE alu_sol.als_rut       =alumno.alu_rut
  AND alu_car                 =car_cod
  AND plan_estudio.plan_car_id=carrera.car_cod
  GROUP BY alumno.alu_rut,
    alu_nombres ,
    alu_paterno ,
    alu_materno ,
    car_nom ,
    plan_estudio.plan_id

select * from v_solicitudes_alumno