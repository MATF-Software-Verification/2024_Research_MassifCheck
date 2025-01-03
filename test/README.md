# Test Direktorijum

## Pravila za imenovanje Massif izlaznih fajlova

- Koristite opciju `--massif-out-file` prilikom pokretanja Massif alata kako biste eksplicitno nazvali izlazne fajlove.
- Format za naziv fajlova treba da bude: `massif.out.<test_case_id>`
  - **Primer:** `massif.out.1`, `massif.out.2`, itd.
- Ovo omogućava konzistentnost i olakšava analizu i poređenje testova.

### Primer komande za pokretanje Massif alata:
```bash
valgrind --tool=massif --massif-out-file=massif.out.1 ./program
