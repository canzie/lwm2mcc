# lwm2mcc

A lightweight, portable LWM2M 1.2 client library written in C.

## Goals
- Full LWM2M 1.2 compliance with backwards compatibility to 1.0 and 1.1
- Platform and backend agnostic — pluggable CoAP, DTLS, and platform layers
- Pluggable memory allocator — no hidden malloc, user provides their own or uses stdlib defaults
- Doxygen-documented public API

## Navigation
- [[Project Structure]] — directory layout and module overview
- [[Architecture]] — high-level design and key abstractions
- [[Versioning]] — compile-time LWM2M version selection (Vulkan-style)
- [[Memory Management]] — pluggable allocator interface
- [[Platform Abstraction Layer]] — transport, security, and platform interfaces
- [[Coding Conventions]] — naming, style, and patterns
- [[Roadmap]] — phased development plan
