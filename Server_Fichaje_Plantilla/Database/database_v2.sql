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

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: admins; Type: TABLE; Schema: public; Owner: usuario
--

CREATE TABLE public.admins (
    admin_id integer NOT NULL,
    admin_user character varying NOT NULL,
    admin_password character varying NOT NULL,
    admin_activo boolean DEFAULT true NOT NULL
);


ALTER TABLE public.admins OWNER TO usuario;

--
-- Name: admins_admin_id_seq; Type: SEQUENCE; Schema: public; Owner: usuario
--

CREATE SEQUENCE public.admins_admin_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.admins_admin_id_seq OWNER TO usuario;

--
-- Name: admins_admin_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: usuario
--

ALTER SEQUENCE public.admins_admin_id_seq OWNED BY public.admins.admin_id;


--
-- Name: empleados; Type: TABLE; Schema: public; Owner: usuario
--

CREATE TABLE public.empleados (
    empleado_id character varying(13) NOT NULL,
    empleado_password character varying(4) NOT NULL,
    empleado_activo boolean DEFAULT true NOT NULL,
    empleado_nombre character varying,
    empleado_apellido character varying
);


ALTER TABLE public.empleados OWNER TO usuario;

--
-- Name: empleados_empleado_apellido_seq; Type: SEQUENCE; Schema: public; Owner: usuario
--

CREATE SEQUENCE public.empleados_empleado_apellido_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.empleados_empleado_apellido_seq OWNER TO usuario;

--
-- Name: empleados_empleado_apellido_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: usuario
--

ALTER SEQUENCE public.empleados_empleado_apellido_seq OWNED BY public.empleados.empleado_apellido;


--
-- Name: empleados_empleado_nombre_seq; Type: SEQUENCE; Schema: public; Owner: usuario
--

CREATE SEQUENCE public.empleados_empleado_nombre_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.empleados_empleado_nombre_seq OWNER TO usuario;

--
-- Name: empleados_empleado_nombre_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: usuario
--

ALTER SEQUENCE public.empleados_empleado_nombre_seq OWNED BY public.empleados.empleado_nombre;


--
-- Name: fichajes; Type: TABLE; Schema: public; Owner: usuario
--

CREATE TABLE public.fichajes (
    fichaje_id integer NOT NULL,
    empleado_id character varying NOT NULL,
    fichaje_es_entrada boolean NOT NULL,
    fichaje_fecha timestamp with time zone DEFAULT now()
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
-- Name: admins admin_id; Type: DEFAULT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.admins ALTER COLUMN admin_id SET DEFAULT nextval('public.admins_admin_id_seq'::regclass);


--
-- Name: fichajes fichaje_id; Type: DEFAULT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.fichajes ALTER COLUMN fichaje_id SET DEFAULT nextval('public.fichajes_fichaje_id_seq'::regclass);


--
-- Name: admins admins_admin_user_key; Type: CONSTRAINT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.admins
    ADD CONSTRAINT admins_admin_user_key UNIQUE (admin_user);


--
-- Name: admins admins_pkey; Type: CONSTRAINT; Schema: public; Owner: usuario
--

ALTER TABLE ONLY public.admins
    ADD CONSTRAINT admins_pkey PRIMARY KEY (admin_id);


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

