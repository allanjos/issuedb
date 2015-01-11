CREATE TABLE project (
  project_id integer PRIMARY KEY,
  name varchar(255),
  status char(1)
);

CREATE TABLE issue (
  issue_id integer PRIMARY KEY,
  summary varchar(255),
  project_id integer,
  priority smallint,
  status char(1),
  platform integer,
  version varchar(15),
  build varchar(10),
  fixed_in_version varchar(15),
  target_version varchar(15),
  reproducibility integer,
  submission_timestamp integer,
  description varchar(255),
  reproduction_steps varchar(255)
);