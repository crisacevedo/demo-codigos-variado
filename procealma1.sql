

CREATE OR REPLACE
PROCEDURE pro_resumen_sol
IS
  CURSOR c_sol
  IS
    SELECT departamento.dep_cod,
      departamento.dep_nom,
      carrera.car_cod,
      carrera.car_nom,
      COUNT(als_id) total
    FROM departamento,
      carrera,
      alumno,
      alu_sol
    WHERE departamento.dep_cod=carrera.car_dep(+)
    AND carrera.car_cod=alumno.alu_car(+)
    AND alumno.alu_rut=alu_sol.als_rut(+)
    GROUP BY departamento.dep_cod,
      departamento.dep_nom,
      carrera.car_cod,
      carrera.car_nom
    ORDER BY 1,4;   
  v_sol c_sol%rowtype;
BEGIN
  OPEN c_sol;
  LOOP
    FETCH c_sol INTO v_sol;
    EXIT
  WHEN c_sol%notfound;
    BEGIN
      INSERT
      INTO resumen_solicitudes VALUES
        (
          v_sol.dep_cod,
          v_sol.dep_nom,
          v_sol.car_cod,
          v_sol.car_nom ,
          v_sol.total,
          USER,
          sysdate
        );
    EXCEPTION
    WHEN DUP_VAL_ON_INDEX THEN
      UPDATE resumen_solicitudes
      SET res_nro_solic=v_sol.total ,
        res_user       =USER ,
        res_fecha      =sysdate
      WHERE res_cod_dep= v_sol.dep_cod
      AND res_cod_car  = v_sol.car_cod;
      COMMIT;
    END;
  END LOOP;
  CLOSE c_sol;
  
  
  COMMIT;
END;

show error;

begin

pro_resumen_sol;

end;


select * from resumen_solicitudes














