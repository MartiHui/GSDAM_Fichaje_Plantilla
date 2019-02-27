--
-- PostgreSQL database dump
--

-- Dumped from database version 10.6
-- Dumped by pg_dump version 10.6

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: empleados; Type: TABLE; Schema: public; Owner: usuario
--

CREATE TABLE public.empleados (
    empleado_id character varying NOT NULL,
    empleado_password character varying NOT NULL,
    empleado_is_active boolean DEFAULT true NOT NULL
);


ALTER TABLE public.empleados OWNER TO usuario;

--
-- Name: empleados empleados_pkey; Type: CONSTRAINT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.empleados
    ADD CONSTRAINT empleados_pkey PRIMARY KEY (empleado_id);


--
-- PostgreSQL database dump complete
--

