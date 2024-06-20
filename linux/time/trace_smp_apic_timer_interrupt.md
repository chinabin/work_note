使用 `sudo trace-cmd record -p function_graph -g smp_apic_timer_interrupt` 追踪并过滤 `sudo trace-cmd report | grep "[003]"` 得到 core 3 上面的数据。

明显看出，有个 330us 的捣蛋
```
<...>-31972 [003] 157833.426602: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
<...>-31972 [003] 157833.426632: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
<...>-31972 [003] 157834.426632: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
<...>-31972 [003] 157834.426662: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
```




```
           <...>-31972 [003] 157833.426602: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
           <...>-31972 [003] 157833.426602: funcgraph_entry:                   |    irq_enter() {
           <...>-31972 [003] 157833.426602: funcgraph_entry:                   |      rcu_irq_enter() {
           <...>-31972 [003] 157833.426602: funcgraph_entry:                   |        rcu_eqs_exit_common.isra.31() {
           <...>-31972 [003] 157833.426602: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157833.426602: funcgraph_exit:         0.015 us   |          }
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.262 us   |        }
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.430 us   |      }
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |      vtime_common_account_irq_enter() {
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |        vtime_account_user() {
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.018 us   |          }
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |          account_user_time() {
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |            cpuacct_account_field() {
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.014 us   |            }
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |            acct_account_cputime() {
           <...>-31972 [003] 157833.426603: funcgraph_entry:                   |              __acct_update_integrals() {
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.054 us   |              }
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.157 us   |            }
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.359 us   |          }
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.577 us   |        }
           <...>-31972 [003] 157833.426603: funcgraph_exit:         0.677 us   |      }
           <...>-31972 [003] 157833.426604: funcgraph_exit:         1.303 us   |    }
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |    exit_idle() {
           <...>-31972 [003] 157833.426604: funcgraph_exit:         0.013 us   |    }
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |    local_apic_timer_interrupt() {
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |      hrtimer_interrupt() {
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |        _raw_qspin_lock() {
           <...>-31972 [003] 157833.426604: funcgraph_exit:         0.017 us   |        }
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |        ktime_get_update_offsets_now() {
           <...>-31972 [003] 157833.426604: funcgraph_exit:         0.071 us   |        }
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |        __hrtimer_run_queues() {
           <...>-31972 [003] 157833.426604: funcgraph_exit:         0.021 us   |        }
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |        __hrtimer_get_next_event() {
           <...>-31972 [003] 157833.426604: funcgraph_exit:         0.014 us   |        }
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |        tick_program_event() {
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |          clockevents_program_event() {
           <...>-31972 [003] 157833.426604: funcgraph_entry:                   |            ktime_get() {
           <...>-31972 [003] 157833.426604: funcgraph_exit:         0.041 us   |            }
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |            lapic_next_deadline() {
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.047 us   |            }
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.294 us   |          }
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.395 us   |        }
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.977 us   |      }
           <...>-31972 [003] 157833.426605: funcgraph_exit:         1.078 us   |    }
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |    irq_exit() {
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |      vtime_gen_account_irq_exit() {
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |        __vtime_account_system() {
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.017 us   |          }
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.120 us   |        }
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.220 us   |      }
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |      idle_cpu() {
           <...>-31972 [003] 157833.426605: funcgraph_exit:         0.013 us   |      }
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |      tick_nohz_irq_exit() {
           <...>-31972 [003] 157833.426605: funcgraph_entry:                   |        can_stop_full_tick() {
           <...>-31972 [003] 157833.426606: funcgraph_entry:                   |          sched_can_stop_tick() {
           <...>-31972 [003] 157833.426606: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426606: funcgraph_entry:                   |          posix_cpu_timers_can_stop_tick() {
           <...>-31972 [003] 157833.426606: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426606: funcgraph_exit:         0.507 us   |        }
           <...>-31972 [003] 157833.426606: funcgraph_entry:                   |        ktime_get() {
           <...>-31972 [003] 157833.426606: funcgraph_exit:         0.024 us   |        }
           <...>-31972 [003] 157833.426606: funcgraph_entry:                   |        tick_nohz_stop_sched_tick() {
           <...>-31972 [003] 157833.426606: funcgraph_entry:                   |          rcu_needs_cpu() {
           <...>-31972 [003] 157833.426606: funcgraph_exit:         0.114 us   |          }
           <...>-31972 [003] 157833.426606: funcgraph_entry:                   |          get_next_timer_interrupt() {
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |            _raw_qspin_lock() {
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.016 us   |            }
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |            hrtimer_get_next_event() {
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |              _raw_spin_lock_irqsave() {
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.019 us   |              }
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |              _raw_spin_unlock_irqrestore() {
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.019 us   |              }
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.237 us   |            }
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.479 us   |          }
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |          timekeeping_max_deferment() {
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |          scheduler_tick_max_deferment() {
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |            jiffies_to_usecs() {
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.013 us   |            }
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.123 us   |          }
           <...>-31972 [003] 157833.426607: funcgraph_entry:                   |          calc_load_enter_idle() {
           <...>-31972 [003] 157833.426607: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426608: funcgraph_exit:         1.263 us   |        }
           <...>-31972 [003] 157833.426608: funcgraph_exit:         2.092 us   |      }
           <...>-31972 [003] 157833.426608: funcgraph_entry:                   |      rcu_irq_exit() {
           <...>-31972 [003] 157833.426608: funcgraph_entry:                   |        rcu_eqs_enter_common.isra.30() {
           <...>-31972 [003] 157833.426608: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157833.426608: funcgraph_exit:         0.015 us   |          }
           <...>-31972 [003] 157833.426608: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157833.426608: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426608: funcgraph_exit:         0.238 us   |        }
           <...>-31972 [003] 157833.426608: funcgraph_exit:         0.352 us   |      }
           <...>-31972 [003] 157833.426608: funcgraph_exit:         3.174 us   |    }
           <...>-31972 [003] 157833.426608: funcgraph_exit:         5.947 us   |  }
           <...>-31972 [003] 157833.426632: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
           <...>-31972 [003] 157833.426632: funcgraph_entry:                   |    irq_enter() {
           <...>-31972 [003] 157833.426632: funcgraph_entry:                   |      rcu_irq_enter() {
           <...>-31972 [003] 157833.426632: funcgraph_entry:                   |        rcu_eqs_exit_common.isra.31() {
           <...>-31972 [003] 157833.426632: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157833.426632: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426632: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157833.426632: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.238 us   |        }
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.352 us   |      }
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |      vtime_common_account_irq_enter() {
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |        vtime_account_user() {
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.018 us   |          }
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.133 us   |        }
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.242 us   |      }
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.805 us   |    }
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |    exit_idle() {
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.014 us   |    }
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |    local_apic_timer_interrupt() {
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |      hrtimer_interrupt() {
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |        _raw_qspin_lock() {
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.017 us   |        }
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |        ktime_get_update_offsets_now() {
           <...>-31972 [003] 157833.426633: funcgraph_exit:         0.024 us   |        }
           <...>-31972 [003] 157833.426633: funcgraph_entry:                   |        __hrtimer_run_queues() {
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |          __remove_hrtimer() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.017 us   |          }
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |          tick_sched_timer() {
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |            ktime_get() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.025 us   |            }
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |            tick_sched_do_timer() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.013 us   |            }
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |            tick_sched_handle() {
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |              touch_softlockup_watchdog_sched() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.013 us   |              }
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |              update_process_times() {
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |                account_process_tick() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.015 us   |                }
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |                hrtimer_run_queues() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.014 us   |                }
           <...>-31972 [003] 157833.426634: funcgraph_entry:                   |                raise_softirq() {
           <...>-31972 [003] 157833.426634: funcgraph_exit:         0.023 us   |                }
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                rcu_check_callbacks() {
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                  cpu_needs_another_gp() {
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.014 us   |                  }
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                  cpu_needs_another_gp() {
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.048 us   |                  }
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                  invoke_rcu_core() {
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                    raise_softirq() {
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.022 us   |                    }
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.130 us   |                  }
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.558 us   |                }
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                scheduler_tick() {
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                  _raw_qspin_lock() {
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.017 us   |                  }
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                  update_rq_clock.part.77() {
           <...>-31972 [003] 157833.426635: funcgraph_exit:         0.017 us   |                  }
           <...>-31972 [003] 157833.426635: funcgraph_entry:                   |                  __update_cpu_load() {
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.017 us   |                  }
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                  task_tick_fair() {
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                    update_curr() {
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                      update_min_vruntime() {
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.014 us   |                      }
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                      cpuacct_charge() {
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.016 us   |                      }
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.336 us   |                    }
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                    __compute_runnable_contrib.part.57() {
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.013 us   |                    }
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                    update_cfs_rq_blocked_load() {
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.019 us   |                    }
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                    update_cfs_shares() {
           <...>-31972 [003] 157833.426636: funcgraph_exit:         0.016 us   |                    }
           <...>-31972 [003] 157833.426636: funcgraph_entry:                   |                    hrtimer_active() {
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.117 us   |                    }
           <...>-31972 [003] 157833.426637: funcgraph_entry:                   |                    __compute_runnable_contrib.part.57() {
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.014 us   |                    }
           <...>-31972 [003] 157833.426637: funcgraph_exit:         1.170 us   |                  }
           <...>-31972 [003] 157833.426637: funcgraph_entry:                   |                  trigger_load_balance() {
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.013 us   |                  }
           <...>-31972 [003] 157833.426637: funcgraph_exit:         1.746 us   |                }
           <...>-31972 [003] 157833.426637: funcgraph_entry:                   |                run_posix_cpu_timers() {
           <...>-31972 [003] 157833.426637: funcgraph_entry:                   |                  task_cputime() {
           <...>-31972 [003] 157833.426637: funcgraph_entry:                   |                    fetch_task_cputime() {
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.018 us   |                    }
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.120 us   |                  }
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.222 us   |                }
           <...>-31972 [003] 157833.426637: funcgraph_exit:         3.149 us   |              }
           <...>-31972 [003] 157833.426637: funcgraph_entry:                   |              profile_tick() {
           <...>-31972 [003] 157833.426637: funcgraph_exit:         0.013 us   |              }
           <...>-31972 [003] 157833.426637: funcgraph_exit:         3.461 us   |            }
           <...>-31972 [003] 157833.426638: funcgraph_exit:         3.797 us   |          }
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |          _raw_qspin_lock() {
           <...>-31972 [003] 157833.426638: funcgraph_exit:         0.017 us   |          }
           <...>-31972 [003] 157833.426638: funcgraph_exit:         4.130 us   |        }
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |        __hrtimer_get_next_event() {
           <...>-31972 [003] 157833.426638: funcgraph_exit:         0.014 us   |        }
           <...>-31972 [003] 157833.426638: funcgraph_exit:         4.563 us   |      }
           <...>-31972 [003] 157833.426638: funcgraph_exit:         4.662 us   |    }
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |    irq_exit() {
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |      vtime_gen_account_irq_exit() {
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |        __vtime_account_system() {
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157833.426638: funcgraph_exit:         0.018 us   |          }
           <...>-31972 [003] 157833.426638: funcgraph_exit:         0.122 us   |        }
           <...>-31972 [003] 157833.426638: funcgraph_exit:         0.223 us   |      }
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |      do_softirq() {
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |        __do_softirq() {
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |          msecs_to_jiffies() {
           <...>-31972 [003] 157833.426638: funcgraph_exit:         0.015 us   |          }
           <...>-31972 [003] 157833.426638: funcgraph_entry:                   |          vtime_common_account_irq_enter() {
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |            vtime_account_user() {
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |              get_vtime_delta() {
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.017 us   |              }
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.120 us   |            }
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.221 us   |          }
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |          run_timer_softirq() {
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |            _raw_spin_lock_irq() {
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.018 us   |            }
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.121 us   |          }
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |          rcu_bh_qs() {
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |          rcu_process_callbacks() {
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |            rcu_process_gp_end() {
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.023 us   |            }
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |            check_for_new_grace_period.isra.26() {
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.023 us   |            }
           <...>-31972 [003] 157833.426639: funcgraph_entry:                   |            cpu_needs_another_gp() {
           <...>-31972 [003] 157833.426639: funcgraph_exit:         0.015 us   |            }
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |            rcu_process_gp_end() {
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |              _raw_spin_trylock() {
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.014 us   |              }
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.171 us   |            }
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |            check_for_new_grace_period.isra.26() {
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |              _raw_spin_trylock() {
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.014 us   |              }
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.135 us   |            }
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |            cpu_needs_another_gp() {
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.015 us   |            }
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.947 us   |          }
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |          rcu_bh_qs() {
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |          vtime_gen_account_irq_exit() {
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |            __vtime_account_system() {
           <...>-31972 [003] 157833.426640: funcgraph_entry:                   |              get_vtime_delta() {
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157833.426640: funcgraph_exit:         0.134 us   |            }
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.235 us   |          }
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |          __local_bh_enable() {
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.015 us   |          }
           <...>-31972 [003] 157833.426641: funcgraph_exit:         2.338 us   |        }
           <...>-31972 [003] 157833.426641: funcgraph_exit:         2.458 us   |      }
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |      idle_cpu() {
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.014 us   |      }
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |      tick_nohz_irq_exit() {
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |        can_stop_full_tick() {
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |          sched_can_stop_tick() {
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |          posix_cpu_timers_can_stop_tick() {
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.226 us   |        }
           <...>-31972 [003] 157833.426641: funcgraph_entry:                   |        ktime_get() {
           <...>-31972 [003] 157833.426641: funcgraph_exit:         0.156 us   |        }
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |        tick_nohz_stop_sched_tick() {
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |          rcu_needs_cpu() {
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |          get_next_timer_interrupt() {
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |            _raw_qspin_lock() {
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.017 us   |            }
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |            hrtimer_get_next_event() {
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |              _raw_spin_lock_irqsave() {
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |              _raw_spin_unlock_irqrestore() {
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.020 us   |              }
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.226 us   |            }
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.436 us   |          }
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |          timekeeping_max_deferment() {
           <...>-31972 [003] 157833.426642: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426642: funcgraph_entry:                   |          scheduler_tick_max_deferment() {
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |            jiffies_to_usecs() {
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.012 us   |            }
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.115 us   |          }
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |          calc_load_enter_idle() {
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |          hrtimer_start() {
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |            hrtimer_start_range_ns() {
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |              lock_hrtimer_base.isra.20() {
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |                _raw_spin_lock_irqsave() {
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.018 us   |                }
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.127 us   |              }
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |              enqueue_hrtimer() {
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.016 us   |              }
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |              tick_program_event() {
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |                clockevents_program_event() {
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |                  ktime_get() {
           <...>-31972 [003] 157833.426643: funcgraph_exit:         0.024 us   |                  }
           <...>-31972 [003] 157833.426643: funcgraph_entry:                   |                  lapic_next_deadline() {
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.046 us   |                  }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.262 us   |                }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.363 us   |              }
           <...>-31972 [003] 157833.426644: funcgraph_entry:                   |              _raw_spin_unlock_irqrestore() {
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.921 us   |            }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         1.022 us   |          }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         2.374 us   |        }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         3.030 us   |      }
           <...>-31972 [003] 157833.426644: funcgraph_entry:                   |      rcu_irq_exit() {
           <...>-31972 [003] 157833.426644: funcgraph_entry:                   |        rcu_eqs_enter_common.isra.30() {
           <...>-31972 [003] 157833.426644: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157833.426644: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.226 us   |        }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         0.339 us   |      }
           <...>-31972 [003] 157833.426644: funcgraph_exit:         6.532 us   |    }
           <...>-31972 [003] 157833.426645: funcgraph_exit:       + 12.397 us  |  }
           <...>-31972 [003] 157834.426632: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
           <...>-31972 [003] 157834.426632: funcgraph_entry:                   |    irq_enter() {
           <...>-31972 [003] 157834.426632: funcgraph_entry:                   |      rcu_irq_enter() {
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |        rcu_eqs_exit_common.isra.31() {
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157834.426633: funcgraph_exit:         0.015 us   |          }
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157834.426633: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426633: funcgraph_exit:         0.238 us   |        }
           <...>-31972 [003] 157834.426633: funcgraph_exit:         0.373 us   |      }
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |      vtime_common_account_irq_enter() {
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |        vtime_account_user() {
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157834.426633: funcgraph_exit:         0.018 us   |          }
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |          account_user_time() {
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |            cpuacct_account_field() {
           <...>-31972 [003] 157834.426633: funcgraph_exit:         0.013 us   |            }
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |            acct_account_cputime() {
           <...>-31972 [003] 157834.426633: funcgraph_entry:                   |              __acct_update_integrals() {
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.054 us   |              }
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.155 us   |            }
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.371 us   |          }
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.619 us   |        }
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.724 us   |      }
           <...>-31972 [003] 157834.426634: funcgraph_exit:         1.327 us   |    }
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |    exit_idle() {
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.014 us   |    }
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |    local_apic_timer_interrupt() {
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |      hrtimer_interrupt() {
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |        _raw_qspin_lock() {
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.017 us   |        }
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |        ktime_get_update_offsets_now() {
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.071 us   |        }
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |        __hrtimer_run_queues() {
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.021 us   |        }
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |        __hrtimer_get_next_event() {
           <...>-31972 [003] 157834.426634: funcgraph_exit:         0.014 us   |        }
           <...>-31972 [003] 157834.426634: funcgraph_entry:                   |        tick_program_event() {
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |          clockevents_program_event() {
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |            ktime_get() {
           <...>-31972 [003] 157834.426635: funcgraph_exit:         0.025 us   |            }
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |            lapic_next_deadline() {
           <...>-31972 [003] 157834.426635: funcgraph_exit:         0.046 us   |            }
           <...>-31972 [003] 157834.426635: funcgraph_exit:         0.287 us   |          }
           <...>-31972 [003] 157834.426635: funcgraph_exit:         0.394 us   |        }
           <...>-31972 [003] 157834.426635: funcgraph_exit:         0.971 us   |      }
           <...>-31972 [003] 157834.426635: funcgraph_exit:         1.074 us   |    }
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |    irq_exit() {
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |      vtime_gen_account_irq_exit() {
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |        __vtime_account_system() {
           <...>-31972 [003] 157834.426635: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.018 us   |          }
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.189 us   |        }
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.470 us   |      }
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |      idle_cpu() {
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.013 us   |      }
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |      tick_nohz_irq_exit() {
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |        can_stop_full_tick() {
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |          sched_can_stop_tick() {
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |          posix_cpu_timers_can_stop_tick() {
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.220 us   |        }
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |        ktime_get() {
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.024 us   |        }
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |        tick_nohz_stop_sched_tick() {
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |          rcu_needs_cpu() {
           <...>-31972 [003] 157834.426636: funcgraph_exit:         0.049 us   |          }
           <...>-31972 [003] 157834.426636: funcgraph_entry:                   |          get_next_timer_interrupt() {
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |            _raw_qspin_lock() {
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.018 us   |            }
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |            hrtimer_get_next_event() {
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |              _raw_spin_lock_irqsave() {
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |              _raw_spin_unlock_irqrestore() {
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.019 us   |              }
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.225 us   |            }
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.463 us   |          }
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |          timekeeping_max_deferment() {
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.015 us   |          }
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |          scheduler_tick_max_deferment() {
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |            jiffies_to_usecs() {
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.013 us   |            }
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.114 us   |          }
           <...>-31972 [003] 157834.426637: funcgraph_entry:                   |          calc_load_enter_idle() {
           <...>-31972 [003] 157834.426637: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426637: funcgraph_exit:         1.138 us   |        }
           <...>-31972 [003] 157834.426638: funcgraph_exit:         1.676 us   |      }
           <...>-31972 [003] 157834.426638: funcgraph_entry:                   |      rcu_irq_exit() {
           <...>-31972 [003] 157834.426638: funcgraph_entry:                   |        rcu_eqs_enter_common.isra.30() {
           <...>-31972 [003] 157834.426638: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157834.426638: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426638: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157834.426638: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426638: funcgraph_exit:         0.219 us   |        }
           <...>-31972 [003] 157834.426638: funcgraph_exit:         0.328 us   |      }
           <...>-31972 [003] 157834.426638: funcgraph_exit:         2.867 us   |    }
           <...>-31972 [003] 157834.426638: funcgraph_exit:         5.651 us   |  }
           <...>-31972 [003] 157834.426662: funcgraph_entry:                   |  smp_apic_timer_interrupt() {
           <...>-31972 [003] 157834.426662: funcgraph_entry:                   |    irq_enter() {
           <...>-31972 [003] 157834.426662: funcgraph_entry:                   |      rcu_irq_enter() {
           <...>-31972 [003] 157834.426662: funcgraph_entry:                   |        rcu_eqs_exit_common.isra.31() {
           <...>-31972 [003] 157834.426662: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.219 us   |        }
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.333 us   |      }
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |      vtime_common_account_irq_enter() {
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |        vtime_account_user() {
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.018 us   |          }
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.123 us   |        }
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.224 us   |      }
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.752 us   |    }
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |    exit_idle() {
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.013 us   |    }
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |    local_apic_timer_interrupt() {
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |      hrtimer_interrupt() {
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |        _raw_qspin_lock() {
           <...>-31972 [003] 157834.426663: funcgraph_exit:         0.018 us   |        }
           <...>-31972 [003] 157834.426663: funcgraph_entry:                   |        ktime_get_update_offsets_now() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.023 us   |        }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |        __hrtimer_run_queues() {
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |          __remove_hrtimer() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.017 us   |          }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |          tick_sched_timer() {
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |            ktime_get() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.024 us   |            }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |            tick_sched_do_timer() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.014 us   |            }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |            tick_sched_handle() {
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |              touch_softlockup_watchdog_sched() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.013 us   |              }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |              update_process_times() {
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |                account_process_tick() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.014 us   |                }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |                hrtimer_run_queues() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.014 us   |                }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |                raise_softirq() {
           <...>-31972 [003] 157834.426664: funcgraph_exit:         0.023 us   |                }
           <...>-31972 [003] 157834.426664: funcgraph_entry:                   |                rcu_check_callbacks() {
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                  cpu_needs_another_gp() {
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.015 us   |                  }
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                  cpu_needs_another_gp() {
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.046 us   |                  }
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                  invoke_rcu_core() {
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                    raise_softirq() {
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.028 us   |                    }
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.128 us   |                  }
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.521 us   |                }
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                scheduler_tick() {
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                  _raw_qspin_lock() {
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.017 us   |                  }
           <...>-31972 [003] 157834.426665: funcgraph_entry:                   |                  update_rq_clock.part.77() {
           <...>-31972 [003] 157834.426665: funcgraph_exit:         0.017 us   |                  }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                  __update_cpu_load() {
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.018 us   |                  }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                  task_tick_fair() {
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                    update_curr() {
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                      update_min_vruntime() {
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.014 us   |                      }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                      cpuacct_charge() {
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.016 us   |                      }
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.240 us   |                    }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                    __compute_runnable_contrib.part.57() {
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.013 us   |                    }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                    update_cfs_rq_blocked_load() {
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.019 us   |                    }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                    update_cfs_shares() {
           <...>-31972 [003] 157834.426666: funcgraph_exit:         0.015 us   |                    }
           <...>-31972 [003] 157834.426666: funcgraph_entry:                   |                    hrtimer_active() {
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.136 us   |                    }
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |                    __compute_runnable_contrib.part.57() {
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.014 us   |                    }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         1.006 us   |                  }
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |                  trigger_load_balance() {
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.013 us   |                  }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         1.711 us   |                }
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |                run_posix_cpu_timers() {
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |                  task_cputime() {
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |                    fetch_task_cputime() {
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.019 us   |                    }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.130 us   |                  }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.234 us   |                }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         3.053 us   |              }
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |              profile_tick() {
           <...>-31972 [003] 157834.426667: funcgraph_exit:         0.013 us   |              }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         3.367 us   |            }
           <...>-31972 [003] 157834.426667: funcgraph_exit:         3.671 us   |          }
           <...>-31972 [003] 157834.426667: funcgraph_entry:                   |          _raw_qspin_lock() {
           <...>-31972 [003] 157834.426668: funcgraph_exit:         0.016 us   |          }
           <...>-31972 [003] 157834.426668: funcgraph_exit:         3.983 us   |        }
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |        __hrtimer_get_next_event() {
           <...>-31972 [003] 157834.426668: funcgraph_exit:         0.015 us   |        }
           <...>-31972 [003] 157834.426668: funcgraph_exit:         4.394 us   |      }
           <...>-31972 [003] 157834.426668: funcgraph_exit:         4.490 us   |    }
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |    irq_exit() {
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |      vtime_gen_account_irq_exit() {
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |        __vtime_account_system() {
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |          get_vtime_delta() {
           <...>-31972 [003] 157834.426668: funcgraph_exit:         0.017 us   |          }
           <...>-31972 [003] 157834.426668: funcgraph_exit:         0.118 us   |        }
           <...>-31972 [003] 157834.426668: funcgraph_exit:         0.229 us   |      }
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |      do_softirq() {
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |        __do_softirq() {
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |          msecs_to_jiffies() {
           <...>-31972 [003] 157834.426668: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |          vtime_common_account_irq_enter() {
           <...>-31972 [003] 157834.426668: funcgraph_entry:                   |            vtime_account_user() {
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |              get_vtime_delta() {
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.118 us   |            }
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.220 us   |          }
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |          run_timer_softirq() {
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |            _raw_spin_lock_irq() {
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.017 us   |            }
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.121 us   |          }
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |          rcu_bh_qs() {
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |          rcu_process_callbacks() {
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |            rcu_process_gp_end() {
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.023 us   |            }
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |            check_for_new_grace_period.isra.26() {
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.023 us   |            }
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |            cpu_needs_another_gp() {
           <...>-31972 [003] 157834.426669: funcgraph_exit:         0.015 us   |            }
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |            rcu_process_gp_end() {
           <...>-31972 [003] 157834.426669: funcgraph_entry:                   |              _raw_spin_trylock() {
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.058 us   |              }
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.167 us   |            }
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |            check_for_new_grace_period.isra.26() {
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |              _raw_spin_trylock() {
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.014 us   |              }
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.149 us   |            }
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |            cpu_needs_another_gp() {
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.013 us   |            }
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.957 us   |          }
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |          rcu_bh_qs() {
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |          vtime_gen_account_irq_exit() {
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |            __vtime_account_system() {
           <...>-31972 [003] 157834.426670: funcgraph_entry:                   |              get_vtime_delta() {
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.119 us   |            }
           <...>-31972 [003] 157834.426670: funcgraph_exit:         0.219 us   |          }
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |          __local_bh_enable() {
           <...>-31972 [003] 157834.426671: funcgraph_exit:         0.016 us   |          }
           <...>-31972 [003] 157834.426671: funcgraph_exit:         2.329 us   |        }
           <...>-31972 [003] 157834.426671: funcgraph_exit:         2.442 us   |      }
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |      idle_cpu() {
           <...>-31972 [003] 157834.426671: funcgraph_exit:         0.012 us   |      }
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |      tick_nohz_irq_exit() {
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |        can_stop_full_tick() {
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |          sched_can_stop_tick() {
           <...>-31972 [003] 157834.426671: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |          posix_cpu_timers_can_stop_tick() {
           <...>-31972 [003] 157834.426671: funcgraph_exit:         0.016 us   |          }
           <...>-31972 [003] 157834.426671: funcgraph_exit:         0.219 us   |        }
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |        ktime_get() {
           <...>-31972 [003] 157834.426671: funcgraph_exit:         0.152 us   |        }
           <...>-31972 [003] 157834.426671: funcgraph_entry:                   |        tick_nohz_stop_sched_tick() {
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |          rcu_needs_cpu() {
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |          get_next_timer_interrupt() {
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |            _raw_qspin_lock() {
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.017 us   |            }
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |            hrtimer_get_next_event() {
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |              _raw_spin_lock_irqsave() {
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.018 us   |              }
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |              _raw_spin_unlock_irqrestore() {
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.020 us   |              }
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.241 us   |            }
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.450 us   |          }
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |          timekeeping_max_deferment() {
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.013 us   |          }
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |          scheduler_tick_max_deferment() {
           <...>-31972 [003] 157834.426672: funcgraph_entry:                   |            jiffies_to_usecs() {
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.013 us   |            }
           <...>-31972 [003] 157834.426672: funcgraph_exit:         0.113 us   |          }
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |          calc_load_enter_idle() {
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.012 us   |          }
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |          hrtimer_start() {
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |            hrtimer_start_range_ns() {
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |              lock_hrtimer_base.isra.20() {
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |                _raw_spin_lock_irqsave() {
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.018 us   |                }
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.126 us   |              }
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |              enqueue_hrtimer() {
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.016 us   |              }
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |              tick_program_event() {
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |                clockevents_program_event() {
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |                  ktime_get() {
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.023 us   |                  }
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |                  lapic_next_deadline() {
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.046 us   |                  }
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.259 us   |                }
           <...>-31972 [003] 157834.426673: funcgraph_exit:         0.359 us   |              }
           <...>-31972 [003] 157834.426673: funcgraph_entry:                   |              _raw_spin_unlock_irqrestore() {
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.020 us   |              }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.889 us   |            }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.989 us   |          }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         2.197 us   |        }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         2.843 us   |      }
           <...>-31972 [003] 157834.426674: funcgraph_entry:                   |      rcu_irq_exit() {
           <...>-31972 [003] 157834.426674: funcgraph_entry:                   |        rcu_eqs_enter_common.isra.30() {
           <...>-31972 [003] 157834.426674: funcgraph_entry:                   |          __smp_mb__before_atomic() {
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.014 us   |          }
           <...>-31972 [003] 157834.426674: funcgraph_entry:                   |          __smp_mb__after_atomic() {
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.012 us   |          }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.244 us   |        }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         0.356 us   |      }
           <...>-31972 [003] 157834.426674: funcgraph_exit:         6.373 us   |    }
           <...>-31972 [003] 157834.426674: funcgraph_exit:       + 12.016 us  |  }
```