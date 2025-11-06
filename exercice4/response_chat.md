Explications (ordre des opérations et pourquoi)

Vérification/affichage des IDs au démarrage

On affiche UID réel/effectif et GID réel/effectif pour tracer le comportement. Utile pour le TP.

Récupération de l'UID/GID de nobody

getpwnam("nobody") renvoie pw_uid et pw_gid qu'on utilisera pour changer d'identité.

Attente de 5 secondes

Le TP demande d'attendre 5s avant le changement ; on utilise sleep(5).

Changement d'identité — ordre critique

initgroups(username, gid) : initialise la liste des groupes supplémentaires du processus pour l'utilisateur cible. Important pour respecter l'appartenance aux groupes.

setgid(gid) : change le GID effectif/communément utilisé. On change le groupe avant l'UID parce que si on change l'UID d'abord (de root vers nobody), on perd les privilèges nécessaires pour modifier le GID.

setuid(uid) : change l'UID (effectif et réel). Après setuid (si elle réussit), on perd typiquement la capacité à redevenir root.

Optionnel / recommandé sur Linux : setresuid / setresgid afin d'effacer aussi les saved IDs (pour empêcher toute montée de privilèges ultérieure via seteuid). Le code essaie de les appeler si disponibles.

Vérification : le code essaie seteuid(0) pour s'assurer qu'on ne peut pas regagner l'UID 0 — si ça marche, c'est un problème.

Résumé : initgroups → setgid → setuid est l'ordre sûr.

Attente 60s puis sortie

Le programme fonctionne désormais avec les droits nobody et reste 60s avant de se terminer.

Compilation et exécution (dans VS Code ou terminal)

Ouvre VS Code, crée un fichier drop_priv.c avec le code ci-dessus.

Ouvre un terminal intégré (Terminal → New Terminal).

Compile :
