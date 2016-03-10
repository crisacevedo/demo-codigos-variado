CREATE OR REPLACE
  FUNCTION fun_cat_asig_cont(
      p_categoria  NUMBER,
      p_asignatura NUMBER)
    RETURN NUMBER
  IS
    v_total      NUMBER(10);
    v_defecto    v_total%type;
    
    
  BEGIN
    
    
    IF p_asignatura=0 THEN
      SELECT COUNT(for_categoria_id)
      INTO v_defecto
      FROM forma,
        asig_for
      WHERE for_categoria_id= p_categoria
      AND asig_for.asf_for   =forma.for_id;
      RETURN v_defecto;
      
    ELSE
    
      SELECT COUNT(for_categoria_id)
      INTO v_defecto
      FROM forma,
        asig_for
      WHERE for_categoria_id= p_categoria
      AND asig_for.asf_for   =forma.for_id;
      RETURN v_defecto;
    END IF;
    END fun_cat_asig_cont;



show error;


--llamada válida

select fun_cat_asig_cont(10,0) from dual;









