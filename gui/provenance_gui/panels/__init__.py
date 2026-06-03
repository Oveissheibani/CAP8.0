"""Per-section UI panels.  Each panel owns its widgets and writes into
the shared AppState; the App class wires them onto the window."""
from .base import Panel
from .run_params  import RunParamsPanel
from .generators  import GeneratorSelectPanel
from .particles   import ParticlesPairsPanel
from .acceptance  import AcceptancePanel
from .stages      import StagesPanel
from .ladder      import LadderDesignerPanel
from .parallelism import ParallelismPanel
from .pythia      import PythiaPanel
from .herwig      import HerwigPanel
from .report      import ReportPanel
from .log_pane    import LogPanel
from .genealogy   import GenealogyRequestPanel

__all__ = [
    "Panel",
    "RunParamsPanel", "GeneratorSelectPanel", "ParticlesPairsPanel",
    "AcceptancePanel",
    "StagesPanel", "LadderDesignerPanel", "ParallelismPanel", "PythiaPanel",
    "HerwigPanel", "ReportPanel", "LogPanel", "GenealogyRequestPanel",
]
