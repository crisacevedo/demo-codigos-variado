
 create table auditoria
     (
      aud_trans  varchar2(1),
      aud_valor  number(4), 
      aud_user   varchar2(20),
      aud_fecha  date 
     )
  
  CREATE OR REPLACE TRIGGER tri_aud_carrera AFTER
  INSERT OR
  DELETE OR
  UPDATE ON carrera FOR EACH row BEGIN IF inserting THEN
  INSERT INTO auditoria VALUES
    ('I',:new.car_cod,USER,sysdate
    );
END IF;
IF updating THEN
  INSERT INTO auditoria VALUES
    ('U',:new.car_cod,USER,sysdate
    );
END IF;
IF deleting THEN
  INSERT INTO auditoria VALUES
    ('D',:old.car_cod,USER,sysdate
    );
END IF;
END ; 
  
select * from carrera;  

select * from auditoria 


insert into carrera (cod_car,nombre_car,cod_dep) values (1235,'lemc',1234)

commit;


update carrera set cod_car=4009 where nombre_car='lemc'

delete from carrera where nombre_car='lemc'