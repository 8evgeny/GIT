CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
​
CREATE TABLE IF NOT EXISTS data_sets (
  data_set_id SERIAL PRIMARY KEY, -- 1 .. 2 147 483 647
  data_set_name character varying(255),
  data_set_path character varying(4096)
);	
​
​
​
CREATE TABLE IF NOT EXISTS algorithms (
  alg_id integer NOT NULL PRIMARY KEY,
  alg_name character varying(255)
);
​
​
CREATE TABLE IF NOT EXISTS trajectories (
  trj_id SERIAL NOT NULL PRIMARY KEY, -- 1 .. 2 147 483 647
  data_set_id integer,
  alg_id integer,
​
 CONSTRAINT trajectories_data_set_id_fkey FOREIGN KEY (data_set_id)
      REFERENCES data_sets(data_set_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
​
​
 CONSTRAINT trajectories_alg_id_fkey FOREIGN KEY (alg_id)
      REFERENCES algorithms(alg_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
​
);
​
CREATE TABLE IF NOT EXISTS trajectory_points (
  pnt_id SERIAL NOT NULL PRIMARY KEY, -- 1 .. 2 147 483 647 
  trj_id integer,   
  x double precision,
  y double precision,
  timestamp bigint,
​
 CONSTRAINT trajectory_points_trj_id_fkey FOREIGN KEY (trj_id)
      REFERENCES trajectories(trj_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
​
​
CREATE TABLE IF NOT EXISTS point_attribute (
  pnt_attr_id SERIAL NOT NULL PRIMARY KEY, -- 1 .. 2 147 483 647 
  pnt_id integer,   
  pnt_attr_data char,
​
 CONSTRAINT points_attribute_pnt_id_fkey FOREIGN KEY (pnt_id)
      REFERENCES trajectory_points(trj_point_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
​
​
CREATE TABLE IF NOT EXISTS trajectory_attribute (
  trj_attr_id SERIAL NOT NULL PRIMARY KEY, -- 1 .. 2 147 483 647 
  trj_id integer,   
  trj_attr_data char,
​
 CONSTRAINT trajectory_attribute_trj_id_fkey FOREIGN KEY (trj_id)
      REFERENCES trajectories(trj_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
​
​
CREATE TABLE IF NOT EXISTS images (
  img_id SERIAL NOT NULL PRIMARY KEY, -- 1 .. 2 147 483 647 
  data_set_id integer,   
  image_name character varying(255),
​
 CONSTRAINT images_data_set_id_fkey FOREIGN KEY (data_set_id)
      REFERENCES data_sets(data_set_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
​
​
​
​
CREATE TABLE IF NOT EXISTS sensor ( 
  data_set_id integer,   
  timestamp bigint,
  flag boolean,
​
 CONSTRAINT sensor_data_set_id_fkey FOREIGN KEY (data_set_id)
      REFERENCES data_sets(data_set_id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
