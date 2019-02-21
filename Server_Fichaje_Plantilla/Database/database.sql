 
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
    "empleado_isAdmin" boolean DEFAULT false NOT NULL
);


ALTER TABLE public.empleados OWNER TO usuario;

--
-- Name: fichajes; Type: TABLE; Schema: public; Owner: usuario
--

CREATE TABLE public.fichajes (
    fichaje_id integer NOT NULL,
    empleado_id character varying NOT NULL,
    fecha_entrada timestamp with time zone NOT NULL,
    fecha_salida timestamp with time zone
);


ALTER TABLE public.fichajes OWNER TO usuario;

--
-- Name: fichajes_fichaje_id_seq; Type: SEQUENCE; Schema: public; Owner: usuario
--

CREATE SEQUENCE public.fichajes_fichaje_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.fichajes_fichaje_id_seq OWNER TO usuario;

--
-- Name: fichajes_fichaje_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: usuario
--

ALTER SEQUENCE public.fichajes_fichaje_id_seq OWNED BY public.fichajes.fichaje_id;


--
-- Name: fichajes fichaje_id; Type: DEFAULT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.fichajes ALTER COLUMN fichaje_id SET DEFAULT nextval('public.fichajes_fichaje_id_seq'::regclass);


--
-- Name: empleados empleados_pkey; Type: CONSTRAINT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.empleados
    ADD CONSTRAINT empleados_pkey PRIMARY KEY (empleado_id);


--
-- Name: fichajes fichajes_pkey; Type: CONSTRAINT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.fichajes
    ADD CONSTRAINT fichajes_pkey PRIMARY KEY (fichaje_id);


--
-- Name: fichajes fichajes_empleado_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.fichajes
    ADD CONSTRAINT fichajes_empleado_id_fkey FOREIGN KEY (empleado_id) REFERENCES public.empleados(empleado_id) ON UPDATE CASCADE ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

